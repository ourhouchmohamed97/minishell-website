/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_handler_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mourhouc <mourhouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:21:17 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/19 17:59:32 by mourhouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	handle_dollar_brace(char *input, int *i)
{
	(*i) += 2;
	if (input[*i] == '}' || !input[*i])
		return (ft_putstr_fd("syntax error: bad substitution\n", 2), 0);
	if (input[*i] == '?' && input[*i + 1] != '}' && input[*i + 1] != '?')
		return (ft_putstr_fd("syntax error: bad substitution\n", 2), 0);
	if (input[*i] == '?' && input[*i + 1] == '}')
	{
		*i += 2;
		return (1);
	}
	while (input[*i] && input[*i] != '}')
	{
		if (input[*i] == '\'' || input[*i] == '"')
			return (ft_putstr_fd("syntax error: bad substitution\n", 2), 0);
		if (!is_valid_var_char(input[*i]))
			return (ft_putstr_fd("invalid character in variable name\n", 2), 0);
		(*i)++;
	}
	if (input[*i] != '}')
		return (ft_putstr_fd("syntax error: missing closing `}`\n", 2), 0);
	(*i)++;
	return (1);
}

int	handle_quote(char *input, int *i, char quote_char, int *last)
{
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
	{
		if (quote_char == '"' && input[*i] == '$' && input[*i + 1] == '{')
		{
			if (handle_dollar_brace(input, i) == 0)
				return (0);
		}
		else
			(*i)++;
	}
	if (!input[*i])
	{
		ft_putstr_fd("syntax error: unclosed quote\n", 2);
		return (0);
	}
	(*i)++;
	*last = 0;
	return (1);
}

int	handle_operator(char *input, int *i)
{
	if (input[*i] == '|' && input[*i + 1] == '|')
		(*i) += 2;
	else if (input[*i] == '|')
		(*i)++;
	else if (input[*i] == '&' && input[*i + 1] == '&')
		(*i) += 2;
	else if (input[*i] == '&')
		(*i)++;
	else if (input[*i] == '<' && input[*i + 1] == '<')
		(*i) += 2;
	else if (input[*i] == '>' && input[*i + 1] == '>')
		(*i) += 2;
	else if (input[*i] == '<' || input[*i] == '>')
		(*i)++;
	return (1);
}

int	ft_test(char *input, int j, int last, t_minishell *shell)
{
	if (!input[j])
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		shell->exit_status = 258;
		return (0);
	}
	if (((last == '<' || last == '>') && (input[j] == '|' || input[j] == '&'
				|| input[j] == '<' || input[j] == '>')) || ((last == '|'
				|| last == '&') && (input[j] == '|' || input[j] == '&')))
	{
		operator_error(input, j);
		shell->exit_status = 258;
		return (0);
	}
	return (1);
}

int	parse_operator(t_minishell *shell, char *input, int *i, int *last)
{
	int		j;
	char	last_operator;

	if (*last && (input[*i] == '|' || input[*i] == '&'))
	{
		operator_error(input, *i);
		shell->exit_status = 258;
		return (0);
	}
	last_operator = input[*i];
	handle_operator(input, i);
	j = *i;
	skip_whitespace(input, &j);
	if (!ft_test(input, j, last_operator, shell))
		return (0);
	return ((*last = 1), 1);
}
