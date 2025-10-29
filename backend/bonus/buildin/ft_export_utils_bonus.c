/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:16:11 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:16:12 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static int	ft_stock(t_list_to_array *d)
{
	while (d->curr)
	{
		if (d->curr->value)
		{
			d->tmp = ft_strjoin(d->curr->name, "=");
			d->arr[d->i] = ft_strjoin(d->tmp, d->curr->value);
			free(d->tmp);
			if (!d->arr[d->i])
			{
				free_double_array(d->arr);
				return (0);
			}
			(d->i)++;
		}
		d->curr = d->curr->next;
	}
	return (1);
}

char	**ft_list_to_array(t_env *env_list)
{
	t_list_to_array	d;

	d.curr = env_list;
	d.i = 0;
	d.len = 0;
	while (d.curr)
	{
		if (d.curr->value)
			d.len++;
		d.curr = d.curr->next;
	}
	d.arr = malloc(sizeof(char *) * (d.len + 1));
	if (!d.arr)
		return (NULL);
	d.curr = env_list;
	if (!ft_stock(&d))
		return (NULL);
	return (d.arr[d.i] = NULL, d.arr);
}

void	sorted_insert(t_env **head, t_env *node_to_insert)
{
	t_env	*current;

	if (!*head || ft_strcmp(node_to_insert->name, (*head)->name) < 0)
	{
		node_to_insert->next = *head;
		*head = node_to_insert;
	}
	else
	{
		current = *head;
		while (current->next
			&& ft_strcmp(node_to_insert->name, current->next->name) > 0)
			current = current->next;
		node_to_insert->next = current->next;
		current->next = node_to_insert;
	}
}

t_env	*create_sorted_copy(t_env *main_env_list)
{
	t_env	*sorted_head;
	t_env	*new_copy_node;
	char	*tmp_str;
	char	*full_str;

	sorted_head = NULL;
	while (main_env_list)
	{
		if (main_env_list->value)
		{
			tmp_str = ft_strjoin(main_env_list->name, "=");
			full_str = ft_strjoin(tmp_str, main_env_list->value);
			free(tmp_str);
		}
		else
			full_str = ft_strdup(main_env_list->name);
		new_copy_node = ft_new_env_node(full_str);
		free(full_str);
		if (!new_copy_node)
			return (free_env_list(sorted_head), NULL);
		sorted_insert(&sorted_head, new_copy_node);
		main_env_list = main_env_list->next;
	}
	return (sorted_head);
}

int	ft_is_vide(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] != '\0')
			return (0);
		i++;
	}
	return (1);
}
