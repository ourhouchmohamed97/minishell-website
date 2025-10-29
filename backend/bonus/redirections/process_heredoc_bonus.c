/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:07 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:22:08 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	prepare_all_heredocs(t_tree_node *ast_root, t_minishell *shell)
{
	int	result;

	if (!ast_root)
		return (0);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	result = process_node_heredocs(ast_root, shell);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (result);
}

int	process_node_heredocs(t_tree_node *node, t_minishell *shell)
{
	if (!node)
		return (0);
	if (process_redirections_heredocs(node->redirection, shell) == -1)
		return (-1);
	if (process_node_heredocs(node->left, shell) == -1)
		return (-1);
	if (process_node_heredocs(node->right, shell) == -1)
		return (-1);
	return (0);
}

int	process_redirections_heredocs(t_redirection *redir_list, t_minishell *shell)
{
	t_redirection	*current_redir;
	int				should_expand;

	current_redir = redir_list;
	while (current_redir)
	{
		if (current_redir->type == HEREDOC)
		{
			should_expand = (current_redir->quote_type == 0);
			current_redir->fd = create_single_heredoc(current_redir, shell,
					should_expand, &shell->exit_status);
			if (current_redir->fd == -1)
				return (-1);
		}
		current_redir = current_redir->next;
	}
	return (0);
}

void	handle_heredoc_signal(int signal_num)
{
	struct termios	term;

	if (signal_num == SIGINT)
	{
		if (tcgetattr(STDIN_FILENO, &term) == 0)
		{
			term.c_lflag |= (ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
		}
		write(STDOUT_FILENO, "\n", 1);
		exit(130);
	}
	else if (signal_num == SIGQUIT)
	{
		return ;
	}
}

int	handle_redirection(t_redirection *redir)
{
	if (redir->type == REDIR_IN)
		return (handle_input_redirection(redir));
	else if (redir->type == REDIR_OUT)
		return (handle_output_redirection(redir));
	else if (redir->type == REDIR_APPEND)
		return (handle_append_redirection(redir));
	else if (redir->type == HEREDOC)
		return (handle_heredoc_redirection(redir));
	return (0);
}
