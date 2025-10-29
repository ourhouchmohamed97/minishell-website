/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:20:56 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:20:57 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	free_token_list(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->val);
		free(current);
		current = next;
	}
}

t_tree_node	*free_ast(t_tree_node *node)
{
	if (!node)
		return (NULL);
	cleanup_heredoc_fds(node);
	free_redirections(node->redirection);
	free_ast(node->left);
	free_ast(node->right);
	free_double_array(node->args);
	if (node->cmd)
		free(node->cmd);
	free(node);
	return (NULL);
}

void	free_double_token_list(t_token **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free_token_list(arr[i]);
		i++;
	}
	free(arr);
}

void	free_parser_arrays(t_token **grouped, t_token **postfix)
{
	int	i;

	if (postfix)
	{
		i = 0;
		while (postfix[i])
		{
			if (postfix[i]->type == WORD)
				free(postfix[i]);
			i++;
		}
		free(postfix);
	}
	if (grouped)
	{
		free(grouped);
	}
}
