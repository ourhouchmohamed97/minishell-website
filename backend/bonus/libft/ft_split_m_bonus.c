/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_m_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:19:21 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:19:22 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_c_mot_m(char const *s, char *c)
{
	int		i;
	int		mot;
	char	quote;

	quote = 0;
	i = 0;
	mot = 0;
	while (s[i])
	{
		if (!ft_is_in_mot(s[i], c) && (i == 0 || ft_is_in_mot(s[i - 1], c))
			&& (quote == 0))
			mot++;
		if (s[i] == '\'' || s[i] == '"')
		{
			if (quote == 0)
				quote = s[i];
			else if (quote == s[i])
				quote = 0;
		}
		i++;
	}
	return (mot + 1);
}

static char	*create_cleaned(char *raw)
{
	char	*cleaned;
	int		j;
	int		k;
	int		quotes;

	j = ((k = ((quotes = 0), 0)), -1);
	if (!raw)
		cleaned = malloc(strlen(raw) + 1);
	if (!raw || !cleaned)
		return (NULL);
	while (raw[++j])
	{
		if (raw[j] == '\'' || raw[j] == '"')
		{
			if (quotes == 0)
				quotes = raw[j];
			else if (quotes == raw[j])
				quotes = 0;
			else
				cleaned[k++] = raw[j];
		}
		else
			cleaned[k++] = raw[j];
	}
	return (free(raw), cleaned[k] = '\0', cleaned);
}

static int	ft_m_mot_m(char const *s, char *c, char **p)
{
	int		i;
	int		start;
	char	quotes;

	quotes = ((i = 0), 0);
	while (s[i] && ft_is_in_mot(s[i], c))
		i++;
	start = i;
	while (s[i] && (!ft_is_in_mot(s[i], c) || quotes))
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			if (quotes == 0)
				quotes = s[i];
			else if (quotes == s[i])
				quotes = 0;
		}
		i++;
	}
	*p = create_cleaned(ft_substr(s, start, i - start));
	if (!*p)
	{
		return (0);
	}
	return (i);
}

char	**ft_split_m(char const *s, char *c)
{
	char	**p;
	int		i;
	int		j;

	if (s == NULL)
		return (NULL);
	p = malloc(sizeof(char *) * ft_c_mot_m(s, c));
	if (!p)
		return (NULL);
	i = 0;
	j = 0;
	while (i < ft_c_mot_m(s, c) - 1)
	{
		j = j + ft_m_mot_m(s + j, c, &p[i]);
		if (!p[i])
		{
			while (i > 0)
				free(p[--i]);
			free(p);
			return (NULL);
		}
		i++;
	}
	p[i] = NULL;
	return (p);
}
