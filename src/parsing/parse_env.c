/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:45:04 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/06 12:03:50 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Retrieves the current and previous working directory paths.
/// Scans the environment variables to extract the values of PWD and OLDPWD.
/// @param tools Structure containing environment-related information.
/// @return Always returns 0 after processing.
int	find_pwd(t_tools *tools)
{
	int	i;

	i = 0;
	while (tools->envp[i])
	{
		if (!ft_strncmp(tools->envp[i], "PWD=", 4))
		{
			tools->pwd = ft_substr(tools->envp[i], 4,
					ft_strlen(tools->envp[i]) - 4);
			if (!tools->pwd)
				return (free(tools->pwd), 0);
		}
		if (!ft_strncmp(tools->envp[i], "OLDPWD=", 7))
		{
			tools->old_pwd = ft_substr(tools->envp[i], 7,
					ft_strlen(tools->envp[i]) - 7);
			if (!tools->old_pwd)
				return (free(tools->old_pwd), 0);
		}
		i++;
	}
	return (1);
}

/// @brief Extracts the PATH variable from the environment.
/// Searches the environment variable list for "PATH=" and returns its value.
/// @param envp Array of environment variables.
/// @return A string containing the extracted PATH, 
//	or an empty string if not found.
char	*find_path(char **envp)
{
	int		i;
	char	*path;

	path = NULL;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
			if (!path)
				return (NULL);
			return (path);
		}
		i++;
	}
	return (NULL);
}

static int	process_paths(t_tools *tools, char *path_from_envp)
{
	int		i;
	int		len;
	char	*tmp;

	tools->paths = ft_split(path_from_envp, ':');
	if (!path_from_envp || path_from_envp[0] == '\0' || !tools->paths)
	{
		printf("Error: PATH no encontrado.\n");
		return (0);
	}
	i = 0;
	while (tools->paths[i])
	{
		len = ft_strlen(tools->paths[i]) - 1;
		if (ft_strncmp(&tools->paths[i][len], "/", 1) != 0)
		{
			tmp = ft_strjoin(tools->paths[i], "/");
			if (!tmp)
				return (ft_free_arr(tools->paths), 0);
			free(tools->paths[i]);
			tools->paths[i] = tmp;
		}
		i++;
	}
	return (1);
}

int	parse_envp(t_tools *tools)
{
	char	*path_from_envp;

	path_from_envp = find_path(tools->envp);
	if (!path_from_envp)
		return (0);
	if (!process_paths(tools, path_from_envp))
	{
		free(path_from_envp);
		return (0);
	}
	free(path_from_envp);
	return (1);
}
