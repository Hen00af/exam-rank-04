#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (type != 'r' && type != 'w')
		return (-1);
	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(pipe_fd[0]); // 子は読まない
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(pipe_fd[1]);
		}
		else
		{
			close(pipe_fd[1]); // 子は書かない
			if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(pipe_fd[0]);
		}
		execvp(file, argv);
		exit(EXIT_FAILURE);
	}
	if (type == 'r')
	{
		close(pipe_fd[1]);   // 親は書かない
		return (pipe_fd[0]); // 親が読む
	}
	else
	{
		close(pipe_fd[0]);   // 親は読まない
		return (pipe_fd[1]); // 親が書く
	}
}
int	main(void)
{
	int		fd;
	char	*line;

	fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	while ((line = get_next_line(fd)))
		printf("%s", line);
}
