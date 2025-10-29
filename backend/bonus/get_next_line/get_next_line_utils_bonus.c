/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:23:39 by oeddamou          #+#    #+#             */
/*   Updated: 2024/12/07 13:45:54 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

long	ft_strchr_g(char *p)
{
	long	i;

	i = 0;
	if (p)
	{
		while (p[i])
		{
			if (p[i] == '\n')
				return (i + 1);
			i++;
		}
		return (i);
	}
	return (-1);
}

long	ft_strcpy_g(char *d, char *s, int c)
{
	long	i;

	i = 0;
	if (s)
	{
		i = 0;
		while (s[i])
		{
			d[i] = s[i];
			if (c == 1 && s[i] == '\n')
			{
				i++;
				break ;
			}
			i++;
		}
		d[i] = '\0';
	}
	return (i);
}

char	*ft_substr_g(char **r)
{
	char	*p;
	long	i;
	long	s;

	if (!r || !(*r))
		return (NULL);
	i = ((s = 0), ft_strchr_g(*r));
	while ((*r)[i + s])
		s++;
	if (s == 0)
		return ((free(*r)), *r = NULL, NULL);
	p = malloc(s + 1);
	if (!p)
		return (free(*r), (*r) = NULL, NULL);
	ft_strcpy_g(p, (*r) + i, 0);
	return (free(*r), *r = NULL, p);
}

int	ft_strjoin_g(char **buf, char *r)
{
	char	*p;
	long	i;
	long	n;

	if (!r)
		return (-1);
	i = ((n = 0), ft_strchr_g(r));
	if (buf && (*buf))
		while ((*buf)[n] != '\0')
			n++;
	p = malloc(i + n + 1);
	if (!p)
		return (-1);
	ft_strcpy_g(p, *buf, 0);
	ft_strcpy_g(p + n, r, 1);
	*buf = ((free(*buf)), p);
	if (p[n + i - 1] == '\n')
		return (1);
	return (0);
}

char	*ft_read_line(int fd, char **line)
{
	char	*r;
	long	s;

	r = ((s = 0), malloc(BUFFER_SIZE + 1));
	if (!r)
		return ((free(*line)), (*line = NULL), NULL);
	while (!s)
	{
		s = read(fd, r, BUFFER_SIZE);
		if (s == -1 || s == 0)
			break ;
		r[s] = '\0';
		s = ft_strjoin_g(line, r);
	}
	if (s == 0 && line && *line)
	{
		r = ((free(r)), *line);
		s = ((*line = NULL), ft_strjoin_g(line, r));
	}
	if (s == -1 || (s == 0 && !*line))
	{
		*line = ((free(*line)), NULL);
		r = ((free(r)), NULL);
	}
	return (ft_substr_g(&r));
}
