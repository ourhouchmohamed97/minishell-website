/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_to_list_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:15:58 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:15:59 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
}

void	add_node_to_end(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

t_env	*find_env_node(t_env *head, const char *name)
{
	while (head)
	{
		if (ft_strcmp(head->name, name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

t_env	*ft_new_env_node(const char *env_str)
{
	t_env	*new_node;
	char	*eq_ptr;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	eq_ptr = ft_strchr(env_str, '=');
	if (!eq_ptr)
	{
		new_node->name = ft_strdup(env_str);
		new_node->value = NULL;
	}
	else
	{
		new_node->name = ft_strndup(env_str, eq_ptr - env_str);
		new_node->value = ft_strdup(eq_ptr + 1);
	}
	if (!new_node->name || (eq_ptr && !new_node->value))
	{
		free(new_node->name);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node->next = NULL, new_node);
}

t_env	*ft_env_to_list(char **env)
{
	t_env	*head;
	t_env	*new_node;
	int		i;

	head = NULL;
	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_node = ft_new_env_node(env[i]);
		if (!new_node)
		{
			free_env_list(head);
			return (NULL);
		}
		add_node_to_end(&head, new_node);
		i++;
	}
	return (head);
}
