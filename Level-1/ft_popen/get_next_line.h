/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shattori <shattori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 00:43:46 by shattori          #+#    #+#             */
/*   Updated: 2024/12/16 21:20:54 by shattori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# if BUFFER_SIZE > INT_MAX
#  undef BUFFER_SIZE
#  define BUFFER_SIZE INT_MAX
# endif

typedef struct s_str
{
	char	*str;
	size_t	len;
	size_t	capa;
}			t_str;

typedef struct s_buff
{
	char	buf[BUFFER_SIZE];
	char	*bufp;
	int		n;
}			t_buff;

void		*ft_memcpy(void *dst, const void *src, size_t n);
int			ft_getchar(int fd);
int			ft_putchar(t_str *str, char c);
char		*get_next_line(int fd);

#endif /*GET_NEXT_LINE_H*/