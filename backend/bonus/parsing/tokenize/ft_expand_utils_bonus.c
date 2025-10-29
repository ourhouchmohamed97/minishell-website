/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:21:32 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:21:33 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	ft_long_name(char *str, int i, int braces)
{
	int	j;

	j = i;
	if (!str || !*str || i == 0 || (str[i - 1] != '$'
			&& (i < 2 || str[i - 1] != '{' || str[i - 2] != '$')))
		return (0);
	if (str[i] == '?')
	{
		return (1);
	}
	if (ft_isalpha(str[i]) || str[i] == '_' || str[i] == '*')
	{
		i++;
		while (str[i] && ((ft_isalnum(str[i]) || str[i] == '_')
				|| (braces && str[i] != '}')))
		{
			i++;
		}
	}
	return (i - j);
}

char	*ft_compare_env(t_export var, int len)
{
	int	j;

	j = 0;
	if (!var.node || !var.node->val || !var.env || len <= 0)
		return (NULL);
	while (var.env[j])
	{
		if (ft_strncmp(var.env[j], var.node->val + *(var.i), len) == 0
			&& var.env[j][len] == '=')
		{
			return (var.env[j] + len + 1);
		}
		j++;
	}
	return (NULL);
}

int	ft_replace(t_export var, int len_name, char *value)
{
	char	*new_val;
	int		old_len;
	int		new_len;
	int		s;

	if (!var.node || !var.node->val || !value)
		return (0);
	old_len = ft_strlen(var.node->val) - len_name - 2 * var.braces - 1;
	new_len = ft_strlen(value);
	new_val = malloc(old_len + new_len + 1);
	if (!new_val)
		return (0);
	s = old_len + new_len + 1;
	ft_strlcpy(new_val, var.node->val, *(var.i) - var.braces);
	ft_strlcat(new_val, value, s);
	ft_strlcat(new_val, var.node->val + (*(var.i)) + len_name + var.braces, s);
	free(var.node->val);
	var.node->val = new_val;
	*(var.i) += new_len - 1 - var.braces;
	return (1);
}

int	ft_replace_question_mark(t_export var)
{
	char	*exit_status_str;
	int		len;

	if (!var.node || !var.node->val)
		return (0);
	exit_status_str = ft_itoa(var.exit_status);
	if (!exit_status_str)
		return (0);
	len = 1;
	while (var.node->val[*(var.i) + len]
		&& var.node->val[*(var.i) + len] != '}' && var.braces)
	{
		len++;
	}
	if (!ft_replace(var, len, exit_status_str))
	{
		free(exit_status_str);
		return (0);
	}
	free(exit_status_str);
	return (1);
}

int	ft_replace_variable(t_export var)
{
	char	*value;
	int		len;
	int		j;

	if (!var.node || !var.node->val || !var.env)
		return (0);
	if (var.node->val[*(var.i)] == '?')
		return (ft_replace_question_mark(var));
	len = ft_long_name(var.node->val, *(var.i), var.braces);
	value = ft_compare_env(var, len);
	if (!value)
	{
		j = *(var.i) + len + var.braces;
		while (var.node->val[j])
		{
			var.node->val[j - 1 - len - 2 * var.braces] = var.node->val[j];
			j++;
		}
		var.node->val[j - 1 - len - 2 * var.braces] = '\0';
		var.node->need_join = ((*(var.i) -= var.braces + 1), 1);
		return (1);
	}
	if (!ft_replace(var, len, value))
		return (0);
	return (1);
}
