/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_operate_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:44:56 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/18 11:44:57 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	execute_command(t_tree_node *node, char **env, t_minishell *data, int f_p)
{
	if (!node)
		return (1);
	else if (!node->cmd && node->redirection)
		return (execute_redirection_only_command(node));
	else if (node->val && is_builtin(node->val))
		return (run_builtin(node, data, f_p));
	return (exec_external(node, env, f_p));
}

int	execute_and(t_tree_node *node, t_minishell *data)
{
	int	left_status;
	int	right_status;

	if (!node || !node->left || !node->right)
		return (1);
	left_status = execute_tree(node->left, data, 0);
	data->exit_status = left_status;
	if (left_status == 0)
	{
		right_status = execute_tree(node->right, data, 0);
		data->exit_status = right_status;
		return (right_status);
	}
	return (left_status);
}

int	execute_or(t_tree_node *node, t_minishell *data)
{
	int	left_status;
	int	right_status;

	if (!node || !node->left || !node->right)
		return (1);
	left_status = execute_tree(node->left, data, 0);
	data->exit_status = left_status;
	if (left_status != 0)
	{
		right_status = execute_tree(node->right, data, 0);
		data->exit_status = right_status;
		return (right_status);
	}
	return (left_status);
}
