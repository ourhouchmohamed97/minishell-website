/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_word_splitting_after_expansion_bonus.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mourhouc <mourhouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:46:34 by oeddamou          #+#    #+#             */
/*   Updated: 2025/10/04 16:28:43 by mourhouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

t_token	*create_word_token(char *word)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->val = ft_strdup(word);
	if (!new_token->val)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = WORD;
	new_token->quote_type = 0;
	new_token->need_join = 0;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

t_token	*ft_create_token_chain(char **words)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new_token;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (words[i])
	{
		new_token = create_word_token(words[i]);
		if (!new_token)
			return (NULL);
		if (!head)
			head = new_token;
		if (tail)
		{
			tail->next = new_token;
			new_token->prev = tail;
		}
		tail = new_token;
		i++;
	}
	return (head);
}

int	ft_should_split_token(t_token *token)
{
	int	i;
	int	have_space;

	if (token->type != WORD || token->quote_type != 0 || !token->val
		|| !*token->val)
		return (0);
	i = ((have_space = 0), 0);
	while (token->val[i] && ft_isspace(token->val[i]))
		i++;
	if (!token->val[i])
		return (2);
	while (token->val[i])
	{
		if (ft_isspace(token->val[i]))
		{
			while (token->val[i] && ft_isspace(token->val[i]))
				i++;
			if (!token->val[i])
				break ;
			have_space = 1;
			break ;
		}
		i++;
	}
	return (have_space);
}

t_token	*split_single_token(t_token *token, t_token **head)
{
	char	**words;
	t_token	*new_head;
	t_token	*new_tail;
	t_token	*next_node;

	next_node = token->next;
	words = ft_split(token->val, " \n\t\v\f\r");
	if (!words || !words[0] || !words[1])
	{
		if (words)
			free_double_array(words);
		return (next_node);
	}
	new_head = ft_create_token_chain(words);
	free_double_array(words);
	if (!new_head)
		return (next_node);
	new_tail = get_tail_token(new_head);
	link_new_tokens(token->prev, new_head, new_tail, next_node);
	if (token == *head)
		*head = new_head;
	free(token->val);
	free(token);
	return (next_node);
}

void	ft_word_splitting_after_expansion(t_token **head, char **env)
{
	t_token	*current;
	int		should_split;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		should_split = ft_should_split_token(current);
		if (should_split == 2)
		{
			current->val[0] = ' ';
			current->val[1] = '\0';
			current = current->next;
		}
		else if (should_split > 0)
			current = split_single_token(current, head);
		else
			current = current->next;
	}
	ft_handle_tilde(head, env);
	ft_wildcard_expand(head);
}