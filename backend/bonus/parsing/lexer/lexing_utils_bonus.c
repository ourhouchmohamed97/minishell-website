/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:21:21 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:21:22 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

void	skip_whitespace(char *s, int *i)
{
	while (s[*i] && (s[*i] == ' ' || (s[*i] >= 9 && s[*i] <= 13)))
		(*i)++;
}

int	is_valid_var_char(char c)
{
	return (c == '_' || c == '?' || (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

void	skip_word(char *input, int *i)
{
	while (input[*i] && !ft_isspace((unsigned char)input[*i])
		&& input[*i] != '|' && input[*i] != '&'
		&& input[*i] != '<' && input[*i] != '>'
		&& input[*i] != '(' && input[*i] != ')'
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
}

void	operator_error(char *s, int i)
{
	if (s[i] == '|' && s[i + 1] == '|')
		ft_putstr_fd("syntax error near unexpected token `||'\n", 2);
	else if (s[i] == '|')
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
	else if (s[i] == '&' && s[i + 1] == '&')
		ft_putstr_fd("syntax error near unexpected token `&&'\n", 2);
	else if (s[i] == '&')
		ft_putstr_fd("syntax error near unexpected token `&'\n", 2);
	else if (s[i] == '<')
		ft_putstr_fd("syntax error near unexpected token `<'\n", 2);
	else if (s[i] == '>')
		ft_putstr_fd("syntax error near unexpected token `>'\n", 2);
}
