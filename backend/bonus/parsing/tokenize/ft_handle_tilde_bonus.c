/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_tilde_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:46:24 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/20 15:27:39 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

void	ft_stock_limiter(char *input, t_token *nd, int *i)
{
	int	start;
	int	len;

	start = *i;
	len = 0;
	if (input[*i] == '|' && input[*i + 1] == '|')
		len = 2;
	else if (input[*i] == '&' && input[*i + 1] == '&')
		len = 2;
	else if (input[*i] == '<' && input[*i + 1] == '<')
		len = 2;
	else if (input[*i] == '>' && input[*i + 1] == '>')
		len = 2;
	else if (ft_limiter(input[*i]))
		len = 1;
	(*i) += len;
	nd->val = malloc(len + 1);
	if (!nd->val)
	{
		nd->need_join = -1;
		return ;
	}
	ft_strlcpy(nd->val, &input[start], len + 1);
	nd->quote_type = 0;
	nd->need_join = 0;
}

int	ft_join_tokens(t_token **head)
{
	t_token	*current;
	t_token	*next;
	char	*new_val;

	current = *head;
	while (current && current->next)
	{
		if (current->need_join > 0)
		{
			next = current->next;
			if (current->need_join == 2)
				current->val[ft_strlen(current->val) - 1] = '\0';
			new_val = ft_strjoin(current->val, next->val);
			if (!new_val)
				return (0);
			free(current->val);
			current->need_join = ((current->val = new_val), next->need_join);
			current->next = ((current->type = next->type), next->next);
			free(next->val);
			free(next);
		}
		else
			current = current->next;
	}
	return (1);
}

int	ft_init_tilde(t_export *var, t_token **head, char **env, int *i)
{
	if (!head || !*head || !env)
		return (0);
	var->node = *head;
	var->i = i;
	var->env = env;
	var->home = getenv("HOME");
	if (!var->home)
		return (0);
	return (1);
}

int	ft_replace_tilde(t_export *var, int *i)
{
	while (var->node->val[*i] && var->braces == 0)
	{
		var->tmp1 = ft_strdup(var->node->val + *i + 1);
		if (!var->tmp1)
			return (0);
		var->node->val[*i] = '\0';
		var->tmp2 = ft_strjoin(var->node->val, var->home);
		if (!var->tmp2)
		{
			free(var->tmp1);
			return (0);
		}
		free(var->node->val);
		var->node->val = ft_strjoin(var->tmp2, var->tmp1);
		free(var->tmp1);
		free(var->tmp2);
		if (!var->node->val)
			return (0);
		*i += ft_strlen(var->home);
		var->braces = ft_get_c(var->node->val, i, '~', var);
	}
	return (1);
}

int	ft_handle_tilde(t_token **head, char **env)
{
	t_export	var;
	int			i;

	if (!ft_init_tilde(&var, head, env, &i))
		return (1);
	var.from_her = 0;
	while (var.node)
	{
		if (var.node->type == WORD && var.node->quote_type == 0)
		{
			i = 0;
			var.braces = ft_get_c(var.node->val, &i, '~', &var);
			if (!ft_replace_tilde(&var, &i))
				return (1);
		}
		var.node = var.node->next;
	}
	return (0);
}
