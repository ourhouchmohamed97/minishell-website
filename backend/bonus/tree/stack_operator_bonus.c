/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_operator_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:43 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:22:44 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	push_node(t_node_stack *stack, t_tree_node *node)
{
	stack->nodes[++stack->top] = node;
}

t_tree_node	*pop_node(t_node_stack *stack)
{
	if (stack->top >= 0)
		return (stack->nodes[stack->top--]);
	return (NULL);
}

void	push_oper(t_oper_stack *stack, t_token *token)
{
	stack->tokens[++stack->top] = token;
}

t_token	*pop_oper(t_oper_stack *stack)
{
	if (stack->top >= 0)
		return (stack->tokens[stack->top--]);
	return (NULL);
}
