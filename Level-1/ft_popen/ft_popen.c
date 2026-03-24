int ft_popen(char **cmd, char **path, char flag)
{
    pid_t pid;
    int fds[2];

    pipe(fds);
    pid = fork();

    if(pid == 0)
    {
        if (flag == 'w')
        {
            close (fds[1]);
            dup2 (fds[0], STDIN_FILENO);
            execvp(path[0], cmd);
        }
        else if (flag == 'r')
        {
            close (fds[0]);
            dup2 (fds[1], STDOUT_FILENO);
            execvp(path[0], cmd);
        }
        exit ( -1);
    }
    else
    {
        if (flag == 'w')
        {
            close (fds[0]);
            return (fds[1]);
        }
        else
        {
            close (fds[1]);
            return (fds[0]);
        }
    }
}