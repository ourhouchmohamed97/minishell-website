/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_variables_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:45:32 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/18 11:45:33 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	ft_init_export(t_export *var, t_token **head, t_minishell *data,
		int *i)
{
	if (!head || !*head || !data || !data->env_list)
		return (0);
	var->node = *head;
	var->i = i;
	var->env = ft_list_to_array(data->env_list);
	if (!var->env)
		return (0);
	var->exit_status = data->exit_status;
	return (1);
}

int	ft_replace_word(t_export *var, int *i)
{
	if (var->node->type == WORD && var->node->quote_type != 1)
	{
		*i = 0;
		var->braces = ft_get_c(var->node->val, i, '$', var);
		while (var->node->val[*i] && var->braces > -1)
		{
			if (!ft_replace_variable(*var))
			{
				free_env(var->env);
				return (0);
			}
			var->braces = ft_get_c(var->node->val, i, '$', var);
		}
	}
	return (1);
}

int	ft_expand_variables(t_token **head, t_minishell *data, int herdoc)
{
	t_export	var;
	int			i;

	if (!ft_init_export(&var, head, data, &i))
		return (1);
	while (var.node)
	{
		if (var.node->type == HEREDOC)
		{
			var.node = var.node->next;
			if (var.node)
				var.node = var.node->next;
			continue ;
		}
		if (!ft_replace_word(&var, &i))
			return (1);
		var.node = var.node->next;
	}
	if (herdoc == 0)
		ft_word_splitting_after_expansion(head, var.env);
	ft_join_tokens(head);
	free_env(var.env);
	return (0);
}
