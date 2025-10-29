/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mourhouc <mourhouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:21:13 by oeddamou          #+#    #+#             */
/*   Updated: 2025/10/06 08:18:52 by mourhouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	check_parentheses_balance(char *input)
{
	int		i;
	int		balance;
	int		in_quotes;
	char	quote_char;

	balance = ((((quote_char = 0), in_quotes = 0), i = 0), 0);
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '"') && !in_quotes)
			quote_char = ((in_quotes = 1), input[i]);
		else if (input[i] == quote_char && in_quotes)
			in_quotes = ((quote_char = 0), 0);
		else if (!in_quotes && input[i] == '(')
			balance++;
		else if (!in_quotes && input[i] == ')')
		{
			balance--;
			if (balance < 0)
				return (ft_put_error(2), 0);
		}
		i++;
	}
	if (balance > 0)
		return (ft_put_error(1), 0);
	return (1);
}

int	handle_quote_with_status(char *input, int *i, char quote_char,
		int *last_was_operator)
{
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
	{
		if (quote_char == '"' && input[*i] == '$' && input[*i + 1] == '{')
		{
			if (!handle_dollar_brace(input, i))
				return (258);
		}
		else
			(*i)++;
	}
	if (!input[*i])
	{
		ft_putstr_fd("syntax error: unclosed quote\n", 2);
		return (258);
	}
	(*i)++;
	*last_was_operator = 0;
	return (0);
}

int	ft_test_parenth(char *input, int *i, int *last_was_operator)
{
	if (input[*i] == '(' || input[*i] == ')')
	{
		(*i)++;
		*last_was_operator = 0;
	}
	return (0);
}

int	process_lexing_token(t_minishell *shell, char *input, int *i,
		int *last_was_operator)
{
	int	quote_status;

	if (input[*i] == '\'' || input[*i] == '"')
	{
		quote_status = handle_quote_with_status(input, i, input[*i],
				last_was_operator);
		if (quote_status != 0)
			return (quote_status);
	}
	else if (input[*i] == '(' || input[*i] == ')')
		return (ft_test_parenth(input, i, last_was_operator));
	else if (input[*i] == '|' || input[*i] == '&' || input[*i] == '<'
		|| input[*i] == '>')
	{
		if (!parse_operator(shell, input, i, last_was_operator))
			return (shell->exit_status);
	}
	else if (!ft_isspace((unsigned char)input[*i]))
	{
		skip_word(input, i);
		*last_was_operator = 0;
	}
	else
		(*i)++;
	return (0);
}

int	ft_lexing(t_minishell *shell, char *input)
{
	int	last_was_operator;
	int	i;
	int	status;

	last_was_operator = 1;
	i = 0;
	if (!check_parentheses_balance(input))
		return (shell->exit_status = 258, 0);
	skip_whitespace(input, &i);
	while (input[i])
	{
		status = process_lexing_token(shell, input, &i, &last_was_operator);
		if (status != 0)
			return (shell->exit_status = status, 0);
		skip_whitespace(input, &i);
	}
	return (1);
}
