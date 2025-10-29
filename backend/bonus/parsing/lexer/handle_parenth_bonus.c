/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parenth_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mourhouc <mourhouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:21:10 by oeddamou          #+#    #+#             */
/*   Updated: 2025/10/04 16:26:19 by mourhouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	only_spaces(char *str, int len)
{
	int	j;

	j = 0;
	while (j < len)
	{
		if (!ft_isspace((unsigned char)str[j]))
			return (0);
		j++;
	}
	return (1);
}

int	ft_put_error(int type)
{
	char	*msg;

	if (type == 1)
		msg = "syntax error: unclosed parenthesis\n";
	else if (type == 2)
		msg = "syntax error near unexpected token `)'\n";
	else if (type == 3)
		msg = "syntax error: unclosed arithmetic parentheses\n";
	else if (type == 4)
		msg = "syntax error: unexpected empty arithmetic expression\n";
	else
		return (1);
	ft_putstr_fd(msg, 2);
	return (0);
}

int	ft_handle_tow_or_more(char *input, int *i)
{
	int	start;
	int	depth;

	(*i) += 2;
	start = *i;
	depth = 2;
	while (input[*i])
	{
		if (input[*i] == '(')
			depth++;
		else if (input[*i] == ')')
		{
			depth--;
			if (depth == 0)
				break ;
		}
		(*i)++;
	}
	if (depth != 0)
		return (ft_put_error(3));
	if (only_spaces(input + start, *i - start))
		return (ft_put_error(4));
	(*i)++;
	return (1);
}

int	handle_parenth(char *input, int *i)
{
	int	count;
	int	start;

	count = 0;
	while (input[*i + count] == '(')
		count++;
	if (count == 1)
	{
		(*i)++;
		start = *i;
		while (input[*i] && input[*i] != ')')
			(*i)++;
		if (!input[*i])
			return (ft_put_error(1));
		if (only_spaces(input + start, *i - start))
			return (ft_put_error(2));
		(*i)++;
		return (1);
	}
	else if (count >= 2)
		return (ft_handle_tow_or_more(input, i));
	return (1);
}
