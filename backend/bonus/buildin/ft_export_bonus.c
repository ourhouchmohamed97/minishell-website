/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:16:08 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/17 20:15:28 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static void	print_export_env(t_env *env_list)
{
	t_env	*temp_sorted_list;
	t_env	*to_free;

	to_free = create_sorted_copy(env_list);
	if (!to_free)
	{
		ft_putstr_fd("minishell: export: malloc failed\n", 2);
		return ;
	}
	temp_sorted_list = to_free;
	while (temp_sorted_list)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(temp_sorted_list->name, 1);
		if (temp_sorted_list->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(temp_sorted_list->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		temp_sorted_list = temp_sorted_list->next;
	}
	free_env_list(to_free);
}

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			break ;
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '+' && str[i + 1] == '=')
		return (2);
	if (str[i] == '=' || str[i] == '\0')
		return (1);
	return (0);
}

static void	ft_helper_add_env(t_env **h, char *arg, t_add_or_update_env *var)
{
	t_env	*node;
	char	*tmp;

	node = find_env_node(*h, var->name);
	if (node && var->eq_ptr)
	{
		if (var->type == 2)
			tmp = ft_strjoin(node->value, var->eq_ptr + 1);
		else
			tmp = ft_strdup(var->eq_ptr + 1);
		free(node->value);
		node->value = tmp;
	}
	else if (!node)
	{
		node = ft_new_env_node(arg);
		if (var->type == 2)
			node->name[ft_strlen(node->name) - 1] = '\0';
		add_node_to_end(h, node);
	}
	free(var->name);
}

static int	add_or_update_env(t_env **head, char *arg)
{
	t_add_or_update_env	var;

	var.type = is_valid_identifier(arg);
	if (!var.type)
		return (0);
	var.eq_ptr = ft_strchr(arg, '=');
	if (var.eq_ptr)
		var.name = ft_strndup(arg, var.eq_ptr - arg);
	else
		var.name = ft_strdup(arg);
	if (var.type == 2)
		var.name[ft_strlen(var.name) - 1] = '\0';
	if (!var.name || !*var.name)
		return (0);
	ft_helper_add_env(head, arg, &var);
	return (1);
}

int	ft_export(t_tree_node *node, t_minishell *data)
{
	int	i;

	data->exit_status = 0;
	if (!node->args[1] || ft_is_vide(node->args))
	{
		print_export_env(data->env_list);
	}
	else
	{
		i = 1;
		while (node->args[i])
		{
			if (node->args[i][0]
				&& !add_or_update_env(&data->env_list, node->args[i]))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(node->args[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				data->exit_status = 1;
			}
			i++;
		}
	}
	return (data->exit_status);
}
