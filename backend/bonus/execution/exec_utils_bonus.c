/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:16:30 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:16:31 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	execute_redirection_only_command(t_tree_node *node)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		perror("dup");
		return (1);
	}
	result = setup_redirections(node);
	restore_fds(saved_stdout, saved_stdin);
	if (result == -1)
		return (1);
	else
		return (0);
}

void	add_redir_to_node(t_tree_node *node, t_redirection *redir)
{
	t_redirection	**current;

	if (!node || !redir)
		return ;
	current = &node->redirection;
	while (*current)
		current = &(*current)->next;
	*current = redir;
}
