/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:20:44 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:20:45 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	free_redirection_node(t_redirection *r)
{
	if (!r)
		return ;
	free(r->filename);
	free(r);
}

void	free_redirections(t_redirection *head)
{
	t_redirection	*tmp;

	while (head)
	{
		tmp = head->next;
		free_redirection_node(head);
		head = tmp;
	}
}
