/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mourhouc <mourhouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:16:41 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/17 21:12:42 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	handle_dir_error(t_tree_node *node, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("Supershell: ", 2);
		ft_putstr_fd(node->val, 2);
		ft_putstr_fd(": is a directory\n", 2);
		free(path);
		exit(126);
	}
}

int	exec_child_process(t_tree_node *node, char **env, char *path)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (setup_redirections(node) == -1)
	{
		free(path);
		exit(1);
	}
	if (!node->args || !node->args[0] || !node->args[0][0])
	{
		free(path);
		exit(0);
	}
	handle_dir_error(node, path);
	execve(path, node->args, env);
	free(path);
	perror("minishell");
	if (errno == EACCES)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(1);
}

int	exec_parent_process(pid_t pid, char *path)
{
	int	status;
	int	sig;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	free(path);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit: 3\n", 8);
		return (128 + sig);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

void	exec_not_found(char *cmd)
{
	ft_putstr_fd("supershell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

int	exec_external(t_tree_node *node, char **env, int from_pipe)
{
	char	*path;
	int		is_direct;

	is_direct = 0;
	path = find_executable(node->args[0], env, &is_direct);
	if (!path)
	{
		if (is_direct)
			print_no_such_file(node->args[0]);
		else
			return (handle_command_not_found(node, from_pipe));
		return (127);
	}
	return (fork_exec_child(node, env, path, from_pipe));
}
