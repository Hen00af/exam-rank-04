#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int set_child_process(char type, char *file[],char *path, char *argv)
{
	if (type == 'r')
		setup_child_read();
	
	else if(type == 'w')
		setup_child_write();
	
	execv(search,path,argv);
	exit(0);
}

int set_child_process(char type)
{

}

int ft_popen(const char *command,const char *path, const char type)
{
	int fds[2];
	pid_t id;
	t_pipe pipe;

	// pipe open
	if(create_pipe(pipe) == -1)
		return (-1);
	//fork
	id = fork();
	//exec child
	setup_child_prosess();
	//exe parents
	setup_parents_process();
	
	return(0);

}
