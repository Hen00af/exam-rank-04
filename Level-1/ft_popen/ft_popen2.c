#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct s_pipe{
    int input_fd;
    int output_fd;
}t_pipe;

static int validate_type(char type)
{
    if (type != 'r' && type != 'w')
        return (-1);
    return (0);
}

static int create_pipe(int pipe_fd[2])
{
    if (pipe(pipe_fd) == -1)
        return (-1);
    return (0);
}

static int setup_child_read_mode(t_pipe pipe)
{
    close(pipe.input_fd);
    if (dup2(pipe.output_fd, STDOUT_FILENO) == -1)
        return (-1);
    close(pipe.output_fd);
    return (0);
}

static int setup_child_write_mode(t_pipe pipe)
{
    close(pipe.output_fd);
    if (dup2(pipe.input_fd, STDIN_FILENO) == -1)
        return (-1);
    close(pipe.input_fd);
    return (0);
}

static int setup_parent_read_mode(t_pipe pipe)
{
    close(pipe.output_fd);
    return (pipe.input_fd);
}

static int setup_parent_write_mode(t_pipe pipe)
{
    close(pipe.input_fd);
    return (pipe.output_fd);
}

static int child_process(const char *file, char *const argv[], char type, t_pipe *pipe)
{
    if (type == 'r')
    {
        if (setup_child_read_mode(*pipe) == -1)
            return (-1);
    }
    else if (type == 'w')
    {
        if (setup_child_write_mode(*pipe) == -1)
            return (-1);
    }
    execvp(file, argv);
    return (-1);
}

static int parent_process(char type, t_pipe *pipe)
{
    int fd;
    
    if (type == 'r')
    {
        fd = setup_parent_read_mode(*pipe);
        if (fd == -1)
            return (-1);
        return (fd);
    }
    else if (type == 'w')
    {
        fd = setup_parent_write_mode(*pipe);
        if (fd == -1)
            return (-1);
        return (fd);
    }
    return (-1);
}

int ft_popen(const char *file, char *const argv[], char type)
{
    int    pipe_fd[2];
    int    ret = -1;
    pid_t pid;

    if (validate_type(type) == -1)
        return (-1);
    if(create_pipe(pipe_fd) == -1)
        return (-1);
    t_pipe pipe = {.input_fd = pipe_fd[0], .output_fd = pipe_fd[1]};
    //fork
    pid = fork();

    //child process
    if (pid == 0)
    {
        child_process(file, argv, type, &pipe);
    }
    //parent prosess
    else if (pid > 0)
    {
        ret = parent_process(type, &pipe);
    }
    return (ret ? ret : -1);
}