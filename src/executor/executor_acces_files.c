/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_acces_files.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:37:56 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/18 11:33:37 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Busca un comando en la ruta dada.  Si el comando es encontrado y es
/// ejecutable, actualiza `command->args[0]`con la ruta completa.
/// @param comando Estructura que contiene los detalles del comando a buscar.
/// @param ruta Ruta en la que se encuentra el comando.
/// @return 1 si el comando es encontrado y ejecutable, sino 0.
int	find_command_in_route(t_command *command, char *path)
{
	char	*joined;
	int		found;

	found = 0;
	joined = ft_strjoin(path, command->args[0]);
	if (access(joined, R_OK | X_OK) != -1)
	{
		free(command->args[0]);
		command->args[0] = joined;
		found = 1;
	}
	else
		free(joined);
	return (found);
}

void	ft_print_err(int err, int fd, t_command *command, t_tools *tools)
{
	ft_putstr_fd(command->args[0], fd);
	if (err == 1)
	{
		ft_putendl_fd(": No such file or directory", fd);
		tools->exit_status = 127;
	}
	else
	{
		ft_putendl_fd(": command not found", fd);
		tools->exit_status = F_CMD_NOT_FOUND;
	}
}

int	validation_status(t_command *command, t_tools *tools)
{
	int		validation_status;
	char	*path;

	path = command->args[0];
	if (access(path, F_OK) == -1)
	{
		fprintf(stderr, "bash: %s: No such file or directory\n", path);
		return (127);
	}
	if (access(path, X_OK) == -1)
	{
		fprintf(stderr, "bash: %s: Permission denied\n", path);
		return (126);
	}
	return (0);
	if (validation_status == 0)
		return (0);
	tools->exit_status = validation_status;
	return (-1);
}

/// @brief Rellena la ruta de ejecución del comando buscando en rutas
/// disponibles en el entorno (`PATH`). Comprueba si el comando es un comando
/// incorporado o un comando externo.
/// @param command
/// @param tools
/// @return 0 si se encuentra el comando o si es un builtin, sino -1.
int	fill_command_from_env(t_command *command, t_tools *tools)
{
	int	found;
	int	i;

	if (is_builtin(command))
		return (0);
	found = 0;
	i = 0;
	if (ft_strncmp(command->args[0], "/", 1) == 0
		|| ft_strncmp(command->args[0], "./", 2) == 0)
		return (validation_status(command, tools));
	if (!tools->paths)
		return (ft_print_err(1, 1, command, tools), -1);
	while (tools->paths[i] && !found)
	{
		if (access(command->args[0], R_OK | X_OK) != -1)
			found = 1;
		else
		{
			found = find_command_in_route(command, tools->paths[i]);
			i++;
		}
	}
	if (!found)
		return (ft_print_err(2, 1, command, tools), -1);
	return (0);
}
