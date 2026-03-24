#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    pid_t pid;
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return (-1);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        close(fd[0]); // fork失敗時は両方閉じる
        close(fd[1]);
        return (-1);
    }

    //child process
    if (pid == 0)
    {
        //parent reads, child writes
        if (type == 'r')
        {
            close (fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
        }
        else if(type == 'w')
        {
            close(fd[1]);
            dup2 (fd[0], STDIN_FILENO);
            close(fd[0]);
        }

        if (execvp(file, argv) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    //parent process
    else
    {
        if (type == 'r')
        {
            close(fd[1]);
            return (fd[0]);
        }
        if (type == 'w')
        {
            close(fd[0]);
            return (fd[1]);
        }
    }
    return (-1);
}

int main(void)
{
    char *argv[] = {"ls", "-la", NULL};
    int fd;
    char buf[1024];
    ssize_t n;

    // 'r' モードで ls -la を実行
    fd = ft_popen("ls", argv, 'r');
    if (fd == -1)
        return (1);

    // パイプから読み込んで標準出力に書き出す
    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        write(STDOUT_FILENO, buf, n);
    }

    close(fd);
    
    // 子プロセスの回収（ゾンビ防止）
    wait(NULL);

    return (0);
}
