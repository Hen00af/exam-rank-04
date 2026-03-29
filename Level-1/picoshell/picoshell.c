#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int picoshell(char **cmds[])
{
    int prev_fd = -1;
    int fds[2];
    pid_t pid;

    for(int i = 0; cmds[i]; i++)
    {
        pipe(fds);
        pid = fork();

        if(pid == 0)
        {
            close(fds[0]);
            if(prev_fd != -1)
                dup2(prev_fd, STDIN_FILENO);
            if (cmds[i + 1])
                dup2 (fds[1], STDOUT_FILENO);
            execvp(cmds[i][0],cmds[i]);
            exit(-1);
        }
        else
        {
            close(fds[1]);
            if (prev_fd != -1)
                close(prev_fd);
            prev_fd = fds[0];
            wait(NULL);
            
        }
    }

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

