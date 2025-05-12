/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:27:37 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/11 17:07:42 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_file_access(const char *path)
{
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
}

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
		tools->exit_status = F_NOT_FILE;
	}
	else
	{
		ft_putendl_fd(": command not found", fd);
		tools->exit_status = F_CMD_NOT_FOUND;
	}
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
	int	validation_status;

	if (is_builtin(command))
		return (0);
	found = 0;
	i = 0;

	// Comprobar si el comando es una ruta absoluta o relativa (empieza con '/' o './')
	if (ft_strncmp(command->args[0], "/", 1) == 0 || ft_strncmp(command->args[0], "./", 2) == 0)
	{
		validation_status = validate_file_access(command->args[0]);
		if (validation_status == 0) // Si la ruta es válida y ejecutable
			return (0); // Ruta válida, continuar con la ejecución
		tools->exit_status = validation_status;
		return (-1); // Ruta inválida o no ejecutable
	}
	if (!tools->paths)
		return (ft_print_err(1, 1, command, tools), -1);
	while (tools->paths[i] && !found)
	{
		if (access(command->args[0], R_OK | X_OK) != -1)
			found = 1;
		else
			found = find_command_in_route(command, tools->paths[i++]);
	}
	if (!found)
		return (ft_print_err(2, 1, command, tools), -1);
	errno = 0;
	return (0);
}

/// get list size
int	get_command_list_size(t_command *list)
{
	int	i;

	i = 0;
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

/// @brief get a pipe from the list based on its position and whether it is the
/// previous pipe or not.
/// It traverses the linked list of pipes to find the relevant one.
/// @param ps Pointer to the head of the pipe list.
/// @param pos Position of the pipe in the sequence.
/// @param is_prev Indicates whether the requested pipe is the previous one (1)
/// or the current one (0).
/// @return Pointer to the requested pipe, or NULL if the previous pipe
t_pipe	*obtain_related_pipe_from_list(t_pipe *ps,
		unsigned int pos, int is_prev)
{
	t_pipe			*curr_pipe;
	unsigned int	i;

	curr_pipe = ps;
	i = 1;
	if (!is_prev)
		i = 0;
	while (i < pos && curr_pipe)
	{
		curr_pipe = curr_pipe->next;
		i++;
	}
	if (pos == 0 && is_prev)
		return (NULL);
	return (curr_pipe);
}
