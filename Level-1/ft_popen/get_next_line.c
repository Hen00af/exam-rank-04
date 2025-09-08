/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shattori <shattori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 00:52:47 by shattori          #+#    #+#             */
/*   Updated: 2025/09/08 21:38:36 by shattori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	t_str	ret;
	char	c;

	ret.str = NULL;
	ret.len = 0;
	ret.capa = 0;
	if (fd < 0)
		return (NULL);
	while (1)
	{
		c = ft_getchar(fd);
		if (c == EOF)
			break ;
		ft_putchar(&ret, c);
		if (c == '\n')
			break ;
	}
	if (c == EOF && ret.len == 0)
		return (NULL);
	if (ret.len > 0)
		ft_putchar(&ret, '\0');
	return (ret.str);
}

// #include <stdio.h>

// int	main(void)
// {
// 	int		fd;
// 	char	*str;
// 	int		i;

// 	i = 0;
// 	fd = open("test.txt", O_RDWR);
// 	while (i++ < 10)
// 	{
// 		str = get_next_line(fd);
// 		printf("%s", str);
// 		free(str);
// 	}
// 	close(fd);
// 	return (0);
// }
