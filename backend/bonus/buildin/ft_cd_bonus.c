/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:15:43 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/18 14:45:23 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static char	*get_env_value(t_env *env_list, const char *name)
{
	t_env	*node;

	node = find_env_node(env_list, name);
	if (node)
		return (node->value);
	return (NULL);
}

int	add_new_node(t_env **head, const char *name, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free (value);
		return (0);
	}
	new_node->name = ft_strdup(name);
	new_node->value = value;
	new_node->next = NULL;
	if (!new_node->name || !new_node->value)
	{
		free_env_list(new_node);
		return (0);
	}
	add_node_to_end(head, new_node);
	return (1);
}

static int	get_target_path(t_tree_node *node, t_minishell *data, char **target)
{
	char	*path_arg;

	if (!node->args[1])
		path_arg = "~";
	else
		path_arg = node->args[1];
	if (ft_strcmp(path_arg, "-") == 0)
	{
		*target = get_env_value(data->env_list, "OLDPWD");
		if (!*target)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
		ft_putendl_fd(*target, 1);
	}
	else if (ft_strcmp(path_arg, "~") == 0 || ft_strcmp(path_arg, "--") == 0)
	{
		*target = get_env_value(data->env_list, "HOME");
		if (!*target)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else
		*target = path_arg;
	return (0);
}

static int	update_pwd_vars(t_minishell *data)
{
	char	*cwd_buffer;
	char	*oldpwd_val;

	oldpwd_val = get_env_value(data->env_list, "PWD");
	if (!set_env_value(&data->env_list, "OLDPWD", oldpwd_val))
		return (1);
	cwd_buffer = getcwd(NULL, 0);
	if (!cwd_buffer)
	{
		ft_putstr_fd("cd: error retrieving current directory\n", 2);
		return (1);
	}
	if (!set_env_value(&data->env_list, "PWD", cwd_buffer))
	{
		free(cwd_buffer);
		return (1);
	}
	free(cwd_buffer);
	return (0);
}

int	ft_cd(t_tree_node *node, t_minishell *data)
{
	char	*target_path;
	int		chdir_ret;

	if (get_target_path(node, data, &target_path) != 0)
		return (1);
	chdir_ret = chdir(target_path);
	if (chdir_ret != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(target_path, 2);
		ft_putstr_fd(": ", 2);
		perror(NULL);
		return (1);
	}
	if (update_pwd_vars(data) != 0)
		return (1);
	return (0);
}
