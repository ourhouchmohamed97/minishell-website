/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_char_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mourhouc <mourhouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:45:44 by oeddamou          #+#    #+#             */
/*   Updated: 2025/10/04 16:28:09 by mourhouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	ft_condition(char *str, int *i, char c, int double_q)
{
	if (c == '~' && double_q % 2 == 0 && (*i == 0 || ft_is_in_mot(str[*i - 1],
				" \t\n\r\v\f")) && (ft_is_in_mot(str[*i + 1], " \t\n\r\v\f/")
			|| str[*i + 1] == '\0'))
		return (0);
	else if (c == '$')
	{
		(*i)++;
		if (str[*i] == '{' && c == '$')
		{
			(*i)++;
			return (1);
		}
		return (0);
	}
	return (-1);
}

int	ft_init_condition(int *s_q, int *d_q, int q_type, char c)
{
	*s_q = 0;
	*d_q = 0;
	if (q_type == 1)
		return (-1);
	else if (q_type == 2 && c == '~')
		return (-1);
	else if (q_type == 2)
		*d_q = 1;
	return (0);
}

int	ft_get_c(char *str, int *i, char c, t_export *v)
{
	int	single_q;
	int	double_q;
	int	condition;

	if (ft_init_condition(&single_q, &double_q, v->node->quote_type, c) == -1)
		return (-1);
	if (!str)
		return (-1);
	while (str[*i])
	{
		if (str[*i] == '\'' && double_q % 2 == 0)
			single_q++;
		else if (str[*i] == '"' && single_q % 2 == 0)
			double_q++;
		else if (str[*i] == c && (single_q % 2 == 0 || v->from_her)
			&& (ft_isalpha(str[(*i) + 1]) || str[(*i) + 1] == '_' || str[(*i)
					+ 1] == '?' || c == '~'))
		{
			condition = ft_condition(str, i, c, double_q);
			if (condition != -1)
				return (condition);
		}
		(*i)++;
	}
	return (-1);
}

int	ft_helper_parenth(t_token *head, t_token_type last_type)
{
	t_token	*tmp;
	int		deapth;

	if (last_type != AND && last_type != OR && last_type != PIPE)
	{
		tmp = ((deapth = 0), head->next);
		while (tmp && (tmp->type != PAREN_CLOSE || deapth != 0))
		{
			if (tmp->type == PAREN_OPEN)
				deapth++;
			else if (tmp->type == PAREN_CLOSE)
				deapth--;
			tmp = tmp->next;
		}
	}
	return (1);
}

int	ft_valide_parenth(t_token *head)
{
	t_token_type	last_type;
	int				i;

	i = 0;
	last_type = head->type;
	while (head)
	{
		if (head->type == PAREN_OPEN)
		{
			if (!ft_helper_parenth(head, last_type))
				return (0);
		}
		last_type = head->type;
		i++;
		head = head->next;
	}
	return (1);
}
