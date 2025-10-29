/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mourhouc <mourhouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:20:52 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/18 12:55:30 by mourhouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	g_sig_recieved = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_sig_recieved = 1;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

int	ft_isspace(unsigned char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

void	run_command_line(char *input, t_minishell *data)
{
	t_tree_node	*tree;

	if (g_sig_recieved)
		data->exit_status = ((g_sig_recieved = 0), 1);
	if (ft_lexing(data, input) == 1 && ft_is_non_empty(input))
	{
		tree = parse_ast(input, data);
		if (tree)
		{
			data->ast = tree;
			if (prepare_all_heredocs(tree, data) == 0)
				data->exit_status = execute_tree(tree, data, 0);
			else
				data->exit_status = 1;
			cleanup_heredoc_fds(tree);
			free_ast(tree);
		}
		else
		{
			if (data->ast && tree != NULL)
				data->ast = free_ast(data->ast);
			data->exit_status = 2;
		}
	}
}

int	main(int ac, char **av, char **env)
{
	t_minishell	data;

	(void)ac;
	(void)av;
	if (init_minishell(&data, env))
		return (1);
	data.is_interactive = isatty(STDIN_FILENO);
	data.exit_status = 0;
	ft_increment_shell_level(&data);
	while (1)
	{
		get_shell_input(data.is_interactive, &data);
		if (*(data.input))
			add_history(data.input);
		run_command_line(data.input, &data);
		free(data.input);
	}
	return (0);
}
