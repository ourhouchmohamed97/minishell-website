/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_heredoc_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:01 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:22:02 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	cleanup_heredoc_fds(t_tree_node *ast_root)
{
	if (!ast_root)
		return ;
	cleanup_node_heredoc_fds(ast_root);
}

void	cleanup_redirection_heredoc_fds(t_redirection *redir_list)
{
	t_redirection	*current_redir;

	current_redir = redir_list;
	while (current_redir)
	{
		if (current_redir->type == HEREDOC && current_redir->fd != -1)
		{
			close(current_redir->fd);
			current_redir->fd = -1;
		}
		current_redir = current_redir->next;
	}
}

void	cleanup_node_heredoc_fds(t_tree_node *node)
{
	if (!node)
		return ;
	cleanup_redirection_heredoc_fds(node->redirection);
	cleanup_node_heredoc_fds(node->left);
	cleanup_node_heredoc_fds(node->right);
}
