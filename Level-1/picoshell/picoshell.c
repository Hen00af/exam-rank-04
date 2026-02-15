#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int picoshell(char **cmds[])
{
    int fd[2];
    int prev_fd = -1;
    pid_t pid;
    int i = 0;

    while (cmds[i])
    {
        // pipe
        if (cmds[i + 1] && pipe(fd) == -1)
        {
            perror("pipe");
            return (1);
        }
        // fork
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return (1);
        }
        if (pid == 0) // --- child process ---
        {
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                    exit(1);
                close(prev_fd);
            }
            if (cmds[i + 1]) // not last command
            {
                close(fd[0]); // close read end
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close(fd[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            perror(cmds[i][0]);
            exit(1);
        }
        else // --- parent process ---
        {
            if (prev_fd != -1)
                close(prev_fd);
            if (cmds[i + 1])
            {
                close(fd[1]);
                prev_fd = fd[0]; // next command reads from here
            }
        }
        i++;
    }
    while (wait(NULL) > 0)
        ;
    return (0);
}

int main(void)
{
    char *ls[] = {"ls", "-l", NULL};
    char *grep[] = {"grep", "c", NULL};
    char *wc[] = {"wc", "-l", NULL};
    char **cmds[] = {ls, grep, wc, NULL};

    picoshell(cmds);
    return 0;
}

