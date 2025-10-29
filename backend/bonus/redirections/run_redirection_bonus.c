/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_redirection_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:10 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:22:11 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	restore_fds(int saved_stdout, int saved_stdin)
{
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
}

void	set_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	run_builtin(t_tree_node *node, t_minishell *data, int f_p)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		perror("dup");
		return (1);
	}
	if (setup_redirections(node) == -1)
	{
		restore_fds(saved_stdout, saved_stdin);
		return (1);
	}
	set_default_signals();
	data->exit_status = exec_builtin(node, data, f_p);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	restore_fds(saved_stdout, saved_stdin);
	return (data->exit_status);
}
