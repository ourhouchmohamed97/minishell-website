/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard_expand_utils_bonus.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:46:29 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/18 11:46:30 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	match(const char *pattern, const char *name)
{
	if (*pattern == '\0')
		return (*name == '\0');
	if (*pattern == '*')
	{
		if (match(pattern + 1, name))
			return (1);
		if (*name != '\0' && *name != '/')
			return (match(pattern, name + 1));
		return (0);
	}
	if (*name != '\0' && *pattern == *name)
		return (match(pattern + 1, name + 1));
	return (0);
}

void	sorted_insert_match(t_match_list **head, t_match_list *new_node)
{
	t_match_list	*current;

	if (!*head || ft_strcmp(new_node->filename, (*head)->filename) < 0)
	{
		new_node->next = *head;
		*head = new_node;
	}
	else
	{
		current = *head;
		while (current->next && ft_strcmp(new_node->filename,
				current->next->filename) > 0)
			current = current->next;
		new_node->next = current->next;
		current->next = new_node;
	}
}

t_match_list	*new_match_node(const char *filename)
{
	t_match_list	*node;

	node = malloc(sizeof(t_match_list));
	if (!node)
		return (NULL);
	node->filename = ft_strdup(filename);
	if (!node->filename)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	free_match_list(t_match_list *head)
{
	t_match_list	*current;
	t_match_list	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->filename);
		free(current);
		current = next;
	}
}

t_token	*get_tail_token(t_token *head)
{
	t_token	*tail;

	if (!head)
		return (NULL);
	tail = head;
	while (tail->next)
		tail = tail->next;
	return (tail);
}
