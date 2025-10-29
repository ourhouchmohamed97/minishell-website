/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:45:03 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/18 13:22:18 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static void	handle_sigint2(int sig)
{
	(void)sig;
	printf("\n");
}

static int	ft_exceute_right(t_minishell *data, t_tree_node *node, t_pipe *pip)
{
	pip->pid_right = fork();
	if (pip->pid_right < 0)
	{
		perror("fork");
		kill(pip->pid_left, SIGKILL);
		close(pip->fd[0]);
		close(pip->fd[1]);
		exit(1);
	}
	if (pip->pid_right == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup2(pip->fd[0], STDIN_FILENO);
		close(pip->fd[0]);
		close(pip->fd[1]);
		if (node->right)
			exit(execute_tree(node->right, data, 1));
		exit(0);
	}
	close(pip->fd[0]);
	close(pip->fd[1]);
	return (0);
}

static int	ft_exceute_left(t_minishell *data, t_tree_node *node, t_pipe *pipe)
{
	signal(SIGINT, handle_sigint2);
	signal(SIGQUIT, SIG_IGN);
	pipe->pid_left = fork();
	if (pipe->pid_left < 0)
	{
		perror("fork");
		close(pipe->fd[0]);
		close(pipe->fd[1]);
		return (1);
	}
	if (pipe->pid_left == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup2(pipe->fd[1], STDOUT_FILENO);
		close(pipe->fd[0]);
		close(pipe->fd[1]);
		if (node->left)
			exit(execute_tree(node->left, data, 1));
		exit(0);
	}
	return (ft_exceute_right(data, node, pipe));
}

int	execute_pipe(t_tree_node *node, t_minishell *data)
{
	t_pipe	v_pipe;

	if (pipe(v_pipe.fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	if (ft_exceute_left(data, node, &v_pipe) != 0)
		return (1);
	waitpid(v_pipe.pid_left, &v_pipe.status_left, 0);
	waitpid(v_pipe.pid_right, &v_pipe.status_right, 0);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	v_pipe.sig_right = 0;
	if (WIFSIGNALED(v_pipe.status_left))
		WTERMSIG(v_pipe.status_left);
	if (WIFSIGNALED(v_pipe.status_right))
		v_pipe.sig_right = WTERMSIG(v_pipe.status_right);
	if (WIFEXITED(v_pipe.status_right))
		return (WEXITSTATUS(v_pipe.status_right));
	if (WIFSIGNALED(v_pipe.status_right))
		return (128 + v_pipe.sig_right);
	return (1);
}
