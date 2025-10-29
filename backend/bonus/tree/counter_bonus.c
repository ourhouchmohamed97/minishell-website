/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counter_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:26 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/20 09:41:12 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	count_args(t_token *tokens)
{
	t_token	*current;
	int		count;

	current = ((count = 0), tokens);
	while (current && !is_operator(current->type))
	{
		if (current->type == WORD)
		{
			count++;
			current = current->next;
		}
		else if (is_redirection(current->type))
		{
			current = current->next;
			if (current && current->type == WORD)
				current = current->next;
		}
		else if (current->type == PAREN_OPEN || current->type == PAREN_CLOSE)
		{
			current = current->next;
		}
		else
			break ;
	}
	return (count);
}

int	count_total_tokens(t_token *tokens)
{
	t_token	*current;
	int		count;

	current = tokens;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	count_token_array(t_token **tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens[count])
		count++;
	return (count);
}

int	count_operators(t_token **tokens)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (is_operator(tokens[i]->type) || tokens[i]->type == PAREN_OPEN
			|| tokens[i]->type == PAREN_CLOSE)
		{
			count++;
		}
		i++;
	}
	return (count);
}

t_token	**tokens_to_infix(t_token *tokens)
{
	t_token	**arr;
	t_token	**grouped;
	int		count;

	if (!tokens)
		return (NULL);
	count = count_total_tokens(tokens);
	arr = fill_token_array(tokens, count);
	if (!arr)
		return (NULL);
	grouped = group_tokens(arr, count);
	free(arr);
	return (grouped);
}
