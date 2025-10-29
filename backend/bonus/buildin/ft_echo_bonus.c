/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:15:48 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:15:49 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static int	ft_is_flag(char *arg)
{
	int	i;

	i = 0;
	if (arg[i++] != '-')
		return (0);
	if (arg[i] != 'n')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_tree_node *node)
{
	int	i;
	int	flag;

	if (node->args == NULL || node->args[0] == NULL || write(1, NULL, 0) == -1)
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	i = 1;
	flag = 0;
	while (node->args[i] && ft_is_flag(node->args[i]) == 1)
	{
		i++;
		flag = 1;
	}
	while (node->args[i])
	{
		ft_putstr_fd(node->args[i], 1);
		if (node->args[i + 1] != NULL)
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (flag == 0)
		ft_putstr_fd("\n", 1);
	return (0);
}
