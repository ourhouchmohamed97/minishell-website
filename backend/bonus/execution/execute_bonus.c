/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:16:37 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:16:38 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (
		ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0);
}

int	exec_builtin(t_tree_node *node, t_minishell *data, int f_p)
{
	if (!node || !node->val)
		return (1);
	if (ft_strcmp(node->val, "cd") == 0)
		return (ft_cd(node, data));
	else if (ft_strcmp(node->val, "echo") == 0)
		return (ft_echo(node));
	else if (ft_strcmp(node->val, "exit") == 0)
		return (ft_exit(node, data, f_p));
	else if (ft_strcmp(node->val, "export") == 0)
		return (ft_export(node, data));
	else if (ft_strcmp(node->val, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(node->val, "unset") == 0)
		return (ft_unset(node, data));
	else if (ft_strcmp(node->val, "env") == 0)
		return (ft_env(node, data));
	return (1);
}

int	execute_tree(t_tree_node *node, t_minishell *data, int from_pipe)
{
	char	**env;

	env = ft_list_to_array(data->env_list);
	if (!node || !env)
	{
		free_env(env);
		return (1);
	}
	if (node->type == WORD)
		data->exit_status = execute_command(node, env, data, from_pipe);
	else if (node->type == PIPE)
		data->exit_status = execute_pipe(node, data);
	else if (node->type == AND)
		data->exit_status = execute_and(node, data);
	else if (node->type == OR)
		data->exit_status = execute_or(node, data);
	else
		data->exit_status = 1;
	free_env(env);
	return (data->exit_status);
}

int	handle_command_not_found(t_tree_node *node, int from_pipe)
{
	pid_t	pid;

	pid = -2;
	if (!from_pipe)
		pid = fork();
	if (pid == 0 || from_pipe)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_not_found(node->args[0]);
		exit(127);
	}
	else if (pid > 0)
		return (exec_parent_process(pid, NULL));
	perror("fork");
	return (1);
}

int	fork_exec_child(t_tree_node *node, char **env, char *path, int from_pipe)
{
	pid_t	pid;

	pid = -2;
	if (!from_pipe)
		pid = fork();
	if (pid == 0 || from_pipe)
		return (exec_child_process(node, env, path));
	else if (!from_pipe && pid > 0)
		return (exec_parent_process(pid, path));
	if (!from_pipe)
		perror("fork");
	return (1);
}
