/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:21:51 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/20 15:41:01 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	ft_helper_substr(char *str, int *i, int flag, char stock_flag)
{
	int	need_join;

	need_join = 0;
	if (flag)
	{
		while (str[*i] && str[*i] != stock_flag)
			(*i)++;
		if (str[(*i) + 1] && !ft_limiter(str[(*i) + 1])
			&& !ft_is_space(str[(*i) + 1]))
			need_join = 1;
	}
	else
	{
		while (str[*i] && !ft_limiter(str[*i]) && !ft_is_space(str[*i])
			&& str[*i] != '\'' && str[*i] != '"')
			(*i)++;
		if (str[*i] && !ft_limiter(str[*i]) && !ft_is_space(str[*i]))
			need_join = 1;
		if (*i > 0 && str[*i - 1] == '$' && (str[*i] == '"' || str[*i] == '\''))
			need_join = 2;
	}
	return (need_join);
}

int	ft_substr_m(char *str, char **copy, int *i, int *quote_type)
{
	int		j;
	int		flag;
	char	stock_flag;
	int		need_join;

	stock_flag = '\0';
	*quote_type = 0;
	flag = ((j = *i), 0);
	if (str[*i] == '\'' || str[*i] == '"')
	{
		stock_flag = str[*i];
		if (stock_flag == '\'')
			*quote_type = 1;
		else
			*quote_type = 2;
		j = ((flag = 1), ++(*i));
	}
	need_join = ft_helper_substr(str, i, flag, stock_flag);
	*copy = malloc(*i - j + 1);
	if (!(*copy))
		return (-1);
	ft_strlcpy(*copy, &str[j], *i - j + 1);
	if (flag != 0)
		(*i)++;
	return (need_join);
}

void	ft_add_type(t_token *node)
{
	node->have_quote = (node->have_quote || node->quote_type);
	node->next = NULL;
	node->type = WORD;
	if (ft_strcmp(node->val, "|") == 0 && !node->quote_type)
		node->type = PIPE;
	else if (ft_strcmp(node->val, "<") == 0 && !node->quote_type)
		node->type = REDIR_IN;
	else if (ft_strcmp(node->val, ">") == 0 && !node->quote_type)
		node->type = REDIR_OUT;
	else if (ft_strcmp(node->val, ">>") == 0 && !node->quote_type)
		node->type = REDIR_APPEND;
	else if (ft_strcmp(node->val, "<<") == 0 && !node->quote_type)
		node->type = HEREDOC;
	else if (ft_strcmp(node->val, "(") == 0 && !node->quote_type)
		node->type = PAREN_OPEN;
	else if (ft_strcmp(node->val, ")") == 0 && !node->quote_type)
		node->type = PAREN_CLOSE;
	else if (ft_strcmp(node->val, "&&") == 0 && !node->quote_type)
		node->type = AND;
	else if (ft_strcmp(node->val, "||") == 0 && !node->quote_type)
		node->type = OR;
}

int	ft_add_back(t_token **head, t_token node)
{
	t_token	*new;
	t_token	*h;

	if (!head)
		return (0);
	h = *head;
	new = malloc(sizeof(t_token));
	if (!new)
	{
		ft_clean_tokens(head);
		return (0);
	}
	*new = node;
	if (!(*head))
	{
		*head = new;
		new->prev = NULL;
		return (1);
	}
	while (h->next)
		h = h->next;
	h->next = new;
	new->prev = h;
	return (1);
}

t_token	*ft_tokenize(char *input)
{
	t_token	*head;
	t_token	nd;
	int		i;

	head = ((i = 0), NULL);
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (ft_skip_space(input, &i) == -1)
			return (head);
		if (ft_limiter(input[i]))
			ft_stock_limiter(input, &nd, &i);
		else
			nd.need_join = ft_substr_m(input, &nd.val, &i, &nd.quote_type);
		if (nd.need_join == -1)
			return ((ft_clean_tokens(&head)), NULL);
		nd.have_quote = (input[i] == '"' || input[i] == '\'');
		ft_add_type(&nd);
		if (!ft_add_back(&head, nd))
			return (NULL);
	}
	if (!ft_valide_parenth(head))
		return (free_token_list(head), NULL);
	return (head);
}
