/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:28:46 by oeddamou          #+#    #+#             */
/*   Updated: 2024/12/04 18:07:51 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*line;
	char		*t;
	static char	*rest;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(rest), rest = NULL, NULL);
	line = rest;
	if (line && line[ft_strchr_g(line) - 1] == '\n')
	{
		t = NULL;
		if (ft_strjoin_g(&t, line) == -1)
			return (free(line), rest = NULL, NULL);
		rest = ft_substr_g(&line);
		free(line);
		line = t;
		t = NULL;
		if (ft_strchr_g(line))
			return (line);
	}
	rest = ft_read_line(fd, &line);
	if (!rest && !line)
		return (NULL);
	return (line);
}
