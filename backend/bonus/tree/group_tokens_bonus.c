/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_tokens_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mourhouc <mourhouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:37 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/15 20:40:25 by mourhouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

t_token	**fill_token_array(t_token *tokens, int count)
{
	t_token	**arr;
	int		i;

	arr = malloc(sizeof(t_token *) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (i < count)
	{
		arr[i] = tokens;
		tokens = tokens->next;
		i++;
	}
	arr[count] = NULL;
	return (arr);
}

int	is_simple_command_start(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == WORD || is_redirection(token->type));
}

t_token	**group_tokens(t_token **arr, int count)
{
	t_token	**grouped;
	t_token	*cmd;
	t_token	*next_token_after_cmd;
	int		i;
	int		j;

	grouped = malloc(sizeof(t_token *) * (count + 1));
	if (!grouped) 
		return (NULL);
	j = ((i = 0), 0);
	while (i < count && arr[i]) 
	{
		if (is_simple_command_start(arr[i])) 
		{
			cmd = create_cmd_token(arr[i], &next_token_after_cmd);
			if (!cmd)
				return (free(grouped), NULL);
			grouped[j++] = cmd;
			while (i < count && arr[i] != next_token_after_cmd) 
				i++;
		}
		else
			grouped[j++] = arr[i++];
	}
	return ((grouped[j] = NULL), grouped);
}

int	add_argument(t_tree_node *node, char **args, int *argc, t_token **cur)
{
	if (*cur && (*cur)->type == WORD)
	{
		args[(*argc)] = ft_strdup((*cur)->val);
		if (!args[(*argc)])
			return (0);
		if (!node->cmd)
		{
			node->cmd = args[(*argc)];
			node->val = args[(*argc)];
		}
		(*argc)++;
		*cur = (*cur)->next;
		return (1);
	}
	return (0);
}

void	process_token(t_tree_node *node, char **args, int *argc, t_token **cur)
{
	t_token_type	rt;

	while (*cur && ((*cur)->type == WORD || is_redirection((*cur)->type)))
	{
		if ((*cur)->type == WORD)
			add_argument(node, args, argc, cur);
		else
		{
			rt = (*cur)->type;
			*cur = (*cur)->next;
			handle_redirection_token(node, cur, rt);
		}
	}
}
