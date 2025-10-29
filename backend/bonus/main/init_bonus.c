/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:20:48 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/20 15:56:41 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

t_tree_node	*parse_ast(char *input, t_minishell *data)
{
	t_tree_node	*tree;
	t_token		*tokens;

	tokens = ft_tokenize(input);
	if (!tokens)
		return (NULL);
	if (ft_expand_variables(&tokens, data, 0))
	{
		ft_putstr_fd("Variable expansion failed\n", 2);
		free_token_list(tokens);
		return (NULL);
	}
	tree = build_tree(tokens);
	free_token_list(tokens);
	return (tree);
}

int	init_minishell(t_minishell *data, char **env)
{
	data->env_list = ft_env_to_list(env);
	if (!data->env_list)
		return (1);
	data->exit_status = 0;
	rl_catch_signals = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

char	*get_shell_input(int is_interactive, t_minishell *data)
{
	if (is_interactive)
		data->input = readline("Superbonus$ ");
	else
		data->input = get_next_line(STDIN_FILENO);
	if (!data->input)
	{
		if (is_interactive)
			ft_putstr_fd("exit\n", 1);
		rl_clear_history();
		free_env_list(data->env_list);
		exit(data->exit_status);
	}
	return (data->input);
}

int	ft_is_non_empty(char *input)
{
	if (!input || !*input)
		return (0);
	while (*input)
	{
		if (!ft_isspace((unsigned char)*input))
			return (1);
		input++;
	}
	return (0);
}
