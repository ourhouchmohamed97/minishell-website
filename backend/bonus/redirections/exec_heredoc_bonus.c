/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:46:40 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/18 11:46:41 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

char	*heredoc_expand(char *input_line, t_minishell *shell)
{
	t_token	temp;
	t_token	*head;
	char	*expanded;

	temp.val = ft_strdup(input_line);
	if (!temp.val)
		return (NULL);
	temp.type = WORD;
	temp.next = NULL;
	temp.quote_type = 0;
	head = &temp;
	ft_expand_variables(&head, shell, 1);
	expanded = temp.val;
	return (expanded);
}

void	execute_heredoc(char *delimiter, int write_fd,
		t_minishell *shell, int should_expand)
{
	char	*input_line;
	char	*tmp;

	while (1)
	{
		if (!shell->is_interactive)
			input_line = get_next_line(STDIN_FILENO);
		else
			input_line = readline("-> ");
		if (!input_line || !ft_strcmp(input_line, delimiter))
		{
			free(input_line);
			break ;
		}
		if (should_expand)
		{
			tmp = heredoc_expand(input_line, shell);
			free(input_line);
			input_line = tmp;
		}
		write(write_fd, input_line, ft_strlen(input_line));
		write(write_fd, "\n", 1);
		free(input_line);
	}
}

int	handle_fork_error(int pipe_fds[2])
{
	perror("fork");
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	return (-1);
}

int	wait_heredoc_child(pid_t child_pid, int pipe_fd, int *exit_status)
{
	int	wait_status;

	waitpid(child_pid, &wait_status, 0);
	if (WIFEXITED(wait_status) && WEXITSTATUS(wait_status) == 130)
	{
		close(pipe_fd);
		*exit_status = 130;
		return (-1);
	}
	return (pipe_fd);
}

int	create_single_heredoc(t_redirection *heredoc_redir,
			t_minishell *shell, int should_expand, int *exit_status)
{
	int		pipe_fds[2];
	pid_t	child_pid;

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (-1);
	}
	child_pid = fork();
	if (child_pid == -1)
		return (handle_fork_error(pipe_fds));
	if (child_pid == 0)
	{
		signal(SIGINT, handle_heredoc_signal);
		signal(SIGQUIT, handle_heredoc_signal);
		close(pipe_fds[0]);
		execute_heredoc(heredoc_redir->filename, pipe_fds[1],
			shell, should_expand);
		close(pipe_fds[1]);
		exit(0);
	}
	close(pipe_fds[1]);
	return (wait_heredoc_child(child_pid, pipe_fds[0], exit_status));
}
