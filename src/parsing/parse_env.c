/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:45:04 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/29 21:54:58 by ampocchi         ###   ########.fr       */
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
				return (0);
		}
		if (!ft_strncmp(tools->envp[i], "OLDPWD=", 7))
		{
			tools->old_pwd = ft_substr(tools->envp[i], 7,
				ft_strlen(tools->envp[i]) - 7);
			if (!tools->pwd)
				return (0);
		}
		i++;
	}
	return (1);
}

/// @brief Extracts the PATH variable from the environment.
/// Searches the environment variable list for "PATH=" and returns its value.
/// @param envp Array of environment variables.
/// @return A string containing the extracted PATH, or an empty string if not found.
char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5));
		i++;
	}
	return (ft_strdup("\0"));
}

/// @brief Parses the environment variables to extract executable paths.
/// Retrieves the PATH variable, splits it into separate directory paths,
/// and ensures each path ends with a '/' for proper command execution.
/// @param tools Structure containing environment and execution utilities.
/// @return Always returns 1 after successful processing.
int	parse_envp(t_tools *tools)
{
	char	*path_from_envp;
	int		i;
	int		len;
	char	*tmp;

	len = 0;
	path_from_envp = find_path(tools->envp);
	tools->paths = ft_split(path_from_envp, ':');
	if (!path_from_envp || path_from_envp[0] == '\0' || !tools->paths)
		return (printf("Error: PATH no encontrado.\n"), 0);
	free(path_from_envp);
	i = 0;
	while (tools->paths[i])
	{
		len = ft_strlen(tools->paths[i])  - 1;
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
