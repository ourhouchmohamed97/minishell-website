/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard_expand_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:45:49 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/18 11:45:50 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

t_match_list	*find_matches(const char *pattern)
{
	DIR				*dir_stream;
	struct dirent	*dir_entry;
	t_match_list	*match_head;
	t_match_list	*new_node;

	match_head = NULL;
	dir_stream = opendir(".");
	if (!dir_stream)
		return (NULL);
	dir_entry = readdir(dir_stream);
	while (dir_entry != NULL)
	{
		if (dir_entry->d_name[0] != '.' || pattern[0] == '.')
		{
			if (match(pattern, dir_entry->d_name))
			{
				new_node = new_match_node(dir_entry->d_name);
				if (new_node)
					sorted_insert_match(&match_head, new_node);
			}
		}
		dir_entry = readdir(dir_stream);
	}
	closedir(dir_stream);
	return (match_head);
}

void	ft_create_token(t_match_list *matches, t_token **head,
	t_token **prev_token)
{
	t_token			*new_token;
	t_match_list	*current_match;

	current_match = matches;
	while (current_match)
	{
		new_token = malloc(sizeof(t_token));
		if (new_token)
		{
			new_token->val = ft_strdup(current_match->filename);
			new_token->type = WORD;
			new_token->quote_type = 0;
			new_token->need_join = 0;
			new_token->next = NULL;
			new_token->prev = *prev_token;
			if (*prev_token)
				(*prev_token)->next = new_token;
			else
				*head = new_token;
			*prev_token = new_token;
		}
		current_match = current_match->next;
	}
}

void	replace_token_with_matches(t_token **current,
	t_match_list *matches, t_token **head)
{
	t_token	*original_token;
	t_token	*prev_token;

	if (!matches)
		return ;
	original_token = *current;
	prev_token = original_token->prev;
	ft_create_token(matches, head, &prev_token);
	if (prev_token)
		prev_token->next = original_token->next;
	if (original_token->next)
		original_token->next->prev = prev_token;
	*current = prev_token;
	free(original_token->val);
	free(original_token);
}

void	ft_wildcard_expand(t_token **head)
{
	t_token			*current;
	t_match_list	*matches;

	current = *head;
	while (current)
	{
		if (ft_strchr(current->val, '*') && !current->quote_type)
		{
			matches = find_matches(current->val);
			if (matches)
			{
				replace_token_with_matches(&current, matches, head);
				free_match_list(matches);
			}
		}
		if (current)
			current = current->next;
	}
}

void	link_new_tokens(t_token *prev, t_token *new_head, t_token *new_tail,
		t_token *next)
{
	if (prev)
		prev->next = new_head;
	if (new_head)
		new_head->prev = prev;
	if (new_tail)
		new_tail->next = next;
	if (next)
		next->prev = new_tail;
}
