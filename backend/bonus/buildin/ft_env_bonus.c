/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:15:54 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:15:55 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	set_env_value(t_env **head, const char *name, const char *value)
{
	t_env	*node;
	char	*new_value;

	if (!value)
		new_value = NULL;
	else
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return (0);
	}
	node = find_env_node(*head, name);
	if (node)
	{
		free(node->value);
		node->value = new_value;
	}
	else
		return (add_new_node(head, name, new_value));
	return (1);
}

void	ft_increment_shell_level(t_minishell *data)
{
	t_env	*shlvl_node;
	int		current_level;
	char	*new_level_str;

	shlvl_node = find_env_node(data->env_list, "SHLVL");
	if (!shlvl_node || !shlvl_node->value || !ft_is_numeric(shlvl_node->value))
		current_level = 1;
	else
		current_level = ft_atoi(shlvl_node->value) + 1;
	if (current_level < 0)
		current_level = 0;
	if (current_level >= 1000)
	{
		ft_putstr_fd("minishell: warning: shell level (", 2);
		ft_putnbr_fd(current_level, 2);
		ft_putstr_fd(") too high, resetting to 1\n", 2);
		current_level = 1;
	}
	new_level_str = ft_itoa(current_level);
	if (!new_level_str)
		return ;
	set_env_value(&data->env_list, "SHLVL", new_level_str);
	free(new_level_str);
}

int	ft_env(t_tree_node *node, t_minishell *data)
{
	t_env	*current;

	if (node->args[1] != NULL)
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(node->args[1], 2);
		ft_putstr_fd("': No such file or directory\n", 2);
		return (127);
	}
	current = data->env_list;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->name, 1);
			ft_putstr_fd("=", 1);
			ft_putendl_fd(current->value, 1);
		}
		current = current->next;
	}
	return (0);
}
