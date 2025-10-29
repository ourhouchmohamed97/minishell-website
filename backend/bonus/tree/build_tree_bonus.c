/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:20 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/17 18:13:14 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	handle_word_token(t_token *current, t_node_stack *stack)
{
	t_tree_node	*cmd_node;

	cmd_node = (t_tree_node *)current->val;
	push_node(stack, cmd_node);
}

int	handle_operator_token(t_token *current, t_node_stack *stack)
{
	t_tree_node	*left;
	t_tree_node	*right;
	t_tree_node	*op_node;

	right = pop_node(stack);
	left = pop_node(stack);
	if (!left || !right)
		return (0);
	op_node = create_operator_node(current->type);
	if (!op_node)
		return (0);
	op_node->left = left;
	op_node->right = right;
	push_node(stack, op_node);
	return (1);
}

void	handle_redirection_token(t_tree_node *node, t_token **cur,
					t_token_type redir_type)
{
	t_redirection	*redir;

	if (*cur && (*cur)->type == WORD)
	{
		redir = malloc(sizeof(t_redirection));
		if (!redir)
			return ;
		redir->type = redir_type;
		redir->filename = ft_strdup((*cur)->val);
		redir->fd = -1;
		redir->quote_type = ((*cur)->quote_type || (*cur)->have_quote);
		redir->next = NULL;
		add_redir_to_node(node, redir);
		*cur = (*cur)->next;
	}
}

t_tree_node	*construct_ast(t_token **postfix)
{
	t_node_stack	*stack;
	t_tree_node		*result;
	int				i;

	stack = create_node_stack(count_token_array(postfix));
	if (!stack)
		return (NULL);
	i = 0;
	while (postfix[i])
	{
		if (postfix[i]->type == WORD)
			handle_word_token(postfix[i], stack);
		else if (is_operator(postfix[i]->type)
			&& !handle_operator_token(postfix[i], stack))
			break ;
		i++;
	}
	result = pop_node(stack);
	free(stack->nodes);
	free(stack);
	return (result);
}

t_tree_node	*build_tree(t_token *tokens)
{
	t_token		**grouped_tokens;
	t_token		**postfix;
	t_tree_node	*ast;

	grouped_tokens = tokens_to_infix(tokens);
	if (!grouped_tokens)
		return (NULL);
	postfix = infix_to_postfix(grouped_tokens);
	if (!postfix)
	{
		free(grouped_tokens);
		return (NULL);
	}
	ast = construct_ast(postfix);
	free_parser_arrays(grouped_tokens, postfix);
	return (ast);
}
