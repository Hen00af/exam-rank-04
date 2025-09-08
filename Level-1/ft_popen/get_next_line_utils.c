/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shattori <shattori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 03:24:06 by shattori          #+#    #+#             */
/*   Updated: 2024/12/13 18:50:26 by shattori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!src || !dst)
		return (NULL);
	while (i < n)
	{
		*((char *)dst + i) = *((char *)src + i);
		i++;
	}
	return (dst);
}

int	ft_getchar(int fd)
{
	static t_buff	buf;
	ssize_t			bytes_read;

	if (buf.n == 0)
	{
		bytes_read = read(fd, buf.buf, BUFFER_SIZE);
		if (bytes_read <= 0)
			return (EOF);
		buf.n = bytes_read;
		buf.bufp = buf.buf;
	}
	buf.n--;
	return ((unsigned char)*buf.bufp++);
}

int	ft_putchar(t_str *str, char c)
{
	size_t	new_capa;
	char	*new_str;

	if (!str)
		return (-1);
	if (str->len + 1 > str->capa)
	{
		if (str->capa == 0)
			new_capa = 1;
		else
			new_capa = str->capa * 2;
		new_str = malloc(new_capa * sizeof(char));
		if (new_str == NULL)
			return (-1);
		if (str->str != NULL)
		{
			ft_memcpy(new_str, str->str, str->len);
			free(str->str);
		}
		str->str = new_str;
		str->capa = new_capa;
	}
	str->str[str->len] = c;
	str->len++;
	return (0);
}
