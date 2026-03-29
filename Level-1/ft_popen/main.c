#include <stdio.h>

int ft_popen(char **cmd, char **path, char flag)
{
    int fds[2];
    pid_t pid;

    pipe(fds);
    pid = fork();

    if(pid == 0)
    {
        if(flag == 'w')
        {
            close(fds[1]);
            dup2(fds[0], STDIN_FILENO);
            close(fds[0]);
            execv(cmd, path);
        }
        else
        {
            close(fds[0]);
            dup2(fds[1], STDOUT_FILENO);
            close(fds[1]);
            execv(cmd, path);
        }
    }
    else
    {
        if (flag == 'w')
        {
            close(fds[0]);
            return(fds[1]);
        }
        else
        {
            close(fds[1]);
            return(fds[0]);
        }
    }

}
