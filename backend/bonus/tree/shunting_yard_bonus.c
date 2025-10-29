/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:41 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:22:42 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	pop_until_paren(t_oper_stack *stack, t_token **output, int *out_pos)
{
	while (stack->top >= 0 && stack->tokens[stack->top]->type != PAREN_OPEN)
		output[(*out_pos)++] = stack->tokens[stack->top--];
	if (stack->top >= 0)
		stack->top--;
}

void	pop_ops_by_precedence(t_oper_stack *stack, t_token **output,
					t_token *current, int *out_pos)
{
	while (stack->top >= 0 && stack->tokens[stack->top]->type != PAREN_OPEN
		&& is_operator(stack->tokens[stack->top]->type)
		&& get_precedence(stack->tokens[stack->top]->type)
		>= get_precedence(current->type))
		output[(*out_pos)++] = stack->tokens[stack->top--];
	stack->tokens[++stack->top] = current;
}

void	pop_remaining_ops(t_oper_stack *stack, t_token **output, int *out_pos)
{
	t_token	*tok;

	while (stack->top >= 0)
	{
		tok = stack->tokens[stack->top--];
		if (tok->type != PAREN_OPEN && tok->type != PAREN_CLOSE)
			output[(*out_pos)++] = tok;
	}
}

void	shunting_process_token(t_token *current, t_oper_stack *stack,
				t_token **output, int *out_pos)
{
	if (current->type == WORD)
		output[(*out_pos)++] = current;
	else if (current->type == PAREN_OPEN)
		stack->tokens[++stack->top] = current;
	else if (current->type == PAREN_CLOSE)
		pop_until_paren(stack, output, out_pos);
	else if (is_operator(current->type))
		pop_ops_by_precedence(stack, output, current, out_pos);
}

t_token	**infix_to_postfix(t_token **tokens)
{
	t_oper_stack	*op_stack;
	t_token			**output;
	int				out_pos;
	int				i;

	output = malloc(sizeof(t_token *) * (count_token_array(tokens) + 1));
	if (!output)
		return (NULL);
	op_stack = create_oper_stack(count_operators(tokens));
	if (!op_stack)
		return (NULL);
	out_pos = 0;
	i = 0;
	while (tokens[i])
		shunting_process_token(tokens[i++], op_stack, output, &out_pos);
	pop_remaining_ops(op_stack, output, &out_pos);
	output[out_pos] = NULL;
	free(op_stack->tokens);
	free(op_stack);
	return (output);
}
