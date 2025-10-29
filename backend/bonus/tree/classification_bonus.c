/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classification_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:46:51 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/20 09:19:14 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	get_precedence(t_token_type type)
{
	if (type == OR || type == AND)
		return (1);
	if (type == PIPE)
		return (2);
	return (0);
}

bool	is_operator(t_token_type type)
{
	return (type == PIPE || type == AND || type == OR);
}

bool	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
		|| type == HEREDOC);
}

t_tree_node	*init_command_node(void)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->type = WORD;
	node->left = NULL;
	node->right = NULL;
	node->redirection = NULL;
	node->val = NULL;
	node->cmd = NULL;
	node->args = NULL;
	return (node);
}

int	process_command(t_tree_node *node, char **args, t_token **cur,
		t_args *arg_struct)
{
	t_token_type	redir_type;

	while (*cur && !is_operator((*cur)->type))
	{
		if ((*cur)->type == WORD)
		{
			if (arg_struct->argc >= arg_struct->arg_count)
				return (ft_putstr_fd("Error: Too many arguments\n", 2), -1);
			if (!node->cmd)
				node->val = (node->cmd = ft_strdup((*cur)->val), node->cmd);
			args[(arg_struct->argc)++] = ft_strdup((*cur)->val);
			*cur = (*cur)->next;
		}
		else if (is_redirection((*cur)->type))
		{
			*cur = ((redir_type = (*cur)->type), (*cur)->next);
			if (*cur && (*cur)->type == WORD)
				handle_redirection_token(node, cur, redir_type);
			else
				break ;
		}
		else if ((*cur)->type == PAREN_OPEN || (*cur)->type == PAREN_CLOSE)
			*cur = (*cur)->next;
	}
	return (0);
}
