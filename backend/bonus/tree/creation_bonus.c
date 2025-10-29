/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creation_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:34 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/20 09:15:30 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

t_node_stack	*create_node_stack(int capacity)
{
	t_node_stack	*stack;

	stack = malloc(sizeof(t_node_stack));
	if (!stack)
		return (NULL);
	stack->nodes = malloc(sizeof(t_tree_node *) * capacity);
	if (!stack->nodes)
		return (free(stack), NULL);
	stack->top = -1;
	return (stack);
}

t_oper_stack	*create_oper_stack(int capacity)
{
	t_oper_stack	*stack;

	stack = malloc(sizeof(t_oper_stack));
	if (!stack)
		return (NULL);
	stack->tokens = malloc(sizeof(t_token *) * capacity);
	if (!stack->tokens)
		return (free(stack), NULL);
	stack->top = -1;
	return (stack);
}

t_tree_node	*create_operator_node(t_token_type type)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->redirection = NULL;
	node->val = NULL;
	node->cmd = NULL;
	node->args = NULL;
	return (node);
}

t_tree_node	*create_command_with_args_and_redirections(t_token *tokens,
				t_token **next_token)
{
	t_tree_node	*node;
	t_token		*cur;
	char		**args;
	t_args		arg_struct;
	int			i;

	i = 0;
	node = init_command_node();
	if (!node)
		return (NULL);
	arg_struct.arg_count = count_args(tokens);
	args = malloc(sizeof(char *) * (arg_struct.arg_count + 1));
	if (!args)
		return (free(node), NULL);
	arg_struct.argc = 0;
	cur = tokens;
	i = process_command(node, args, &cur, &arg_struct);
	if (i < 0 || (arg_struct.arg_count == 0 && node->redirection == NULL))
		return (free(node), free(args), NULL);
	args[arg_struct.argc] = NULL;
	node->args = args;
	*next_token = cur;
	return (node);
}

t_token	*create_cmd_token(t_token *token, t_token **next)
{
	t_token		*cmd_token;
	t_tree_node	*cmd_node;

	cmd_token = malloc(sizeof(t_token));
	if (!cmd_token)
		return (NULL);
	cmd_token->type = WORD;
	cmd_token->quote_type = 0;
	cmd_token->next = NULL;
	cmd_node = create_command_with_args_and_redirections(token, next);
	if (!cmd_node)
	{
		free(cmd_token);
		return (NULL);
	}
	cmd_token->val = (char *)cmd_node;
	return (cmd_token);
}
