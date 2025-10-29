/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:21:54 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:21:55 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	ft_is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

int	ft_skip_space(char *str, int *i)
{
	while (str[*i] && ft_is_space(str[*i]))
		(*i)++;
	if (str[*i] == '\0')
		return (-1);
	return (*i);
}

int	ft_limiter(char c)
{
	if (c == '|' || c == '(' || c == ')' || c == '>' || c == '<' || c == '&')
		return (1);
	return (0);
}

int	ft_bad_space(char *str)
{
	int	i;

	i = 0;
	if (!ft_is_space(str[i]))
		return (0);
	while (str[i] && ft_is_space(str[i]))
		i++;
	if (!str[i] || ft_limiter(str[i]))
		return (1);
	return (0);
}

void	ft_clean_tokens(t_token **head)
{
	t_token	*h;

	h = *head;
	while (h)
	{
		free(h->val);
		*head = (*head)->next;
		free(h);
		h = *head;
	}
}
