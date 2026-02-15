#include <stdio.h>
#include "ft_popen.h"
#include "get_next_line.h"



int main(void)
{
    int     fd;
    char    *line;

    fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
    while ((line = get_next_line(fd)))
        printf("%s", line);
    return 0;
}
