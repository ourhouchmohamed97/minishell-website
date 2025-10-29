/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:16:45 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:16:46 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

char	*try_direct_path(const char *cmd)
{
	struct stat	st;

	if (!cmd || *cmd == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
				return (ft_strdup(cmd));
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
		}
		return (NULL);
	}
	return (NULL);
}

char	*find_in_path_env(char **env)
{
	int	i;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*search_in_paths(char **paths, const char *cmd)
{
	struct stat	st;
	char		*full_path;
	size_t		len;
	int			i;

	if (!paths || !cmd || *cmd == '\0')
		return (NULL);
	i = 0;
	while (paths[i])
	{
		len = ft_strlen(paths[i]) + ft_strlen(cmd) + 2;
		full_path = malloc(len);
		if (!full_path)
			return (NULL);
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0 && stat(full_path, &st) == 0
			&& !S_ISDIR(st.st_mode))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	print_no_such_file(const char *cmd)
{
	ft_putstr_fd("supershell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

char	*find_executable(char *cmd, char **env, int *is_direct)
{
	char	*direct;
	char	*path_env;
	char	*result;
	char	**paths;
	int		i;

	if (!cmd || *cmd == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		direct = ((*is_direct = 1), try_direct_path(cmd));
		return (direct);
	}
	path_env = find_in_path_env(env);
	if (!path_env)
		return (ft_strdup(cmd));
	if (!path_env || !*path_env)
		return (NULL);
	paths = ft_split(path_env, ":");
	if (!paths)
		return (NULL);
	result = ((i = 0), search_in_paths(paths, cmd));
	while (paths[i])
		free(paths[i++]);
	return (free(paths), result);
}
