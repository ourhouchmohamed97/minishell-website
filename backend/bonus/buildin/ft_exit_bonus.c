/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:16:02 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:16:03 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static int	is_numeric_string(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	cleanup_and_exit(t_minishell *data, int exit_code)
{
	if (data->env_list)
		free_env_list(data->env_list);
	if (data->ast)
		free_ast(data->ast);
	if (data->input)
		free(data->input);
	rl_clear_history();
	exit(exit_code);
}

int	ft_exit(t_tree_node *node, t_minishell *data, int f_p)
{
	int	exit_code;

	if (f_p == 0 && data->is_interactive)
		ft_putendl_fd("exit", 1);
	if (!node->args[1])
		cleanup_and_exit(data, data->exit_status);
	else if (!is_numeric_string(node->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(node->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_and_exit(data, 255);
	}
	else if (node->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	else
	{
		exit_code = ft_atoi(node->args[1]) % 256;
		cleanup_and_exit(data, exit_code);
	}
	return (0);
}
