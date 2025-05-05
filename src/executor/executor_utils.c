/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:27:37 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/05 08:28:40 by ampocchi         ###   ########.fr       */
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
	if (!tools->paths)
	{
		printf("%s: No such file or directory\n", command->args[0]);
		tools->exit_status = F_NOT_FILE;
		return (-1);
	}
	while (tools->paths[i] && !found)
	{
		if (access(command->args[0], R_OK | X_OK) != -1)
			found = 1;
		else
			found = find_command_in_route(command, tools->paths[i++]);
	}
	if (!found)
	{
		printf("%s: command not found\n", command->args[0]);
		tools->exit_status = F_CMD_NOT_FOUND;
		return (-1);
	}
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

/// @brief Actualiza el estado de salida después de ejecutar un comando.
/// Maneja códigos de retorno, errores de comando no encontrado e
/// interrupciones SIGINT.
/// @param status Código de retorno del comando ejecutado.
/// @param tools
void	handle_status(int status, t_tools *tools)
{
	if (WIFEXITED(status))
	{
		tools->exit_status = WEXITSTATUS(status);
		if (status == 256)
			tools->exit_status = 1;
		else if (tools && tools->exit_status == F_CMD_NOT_FOUND)
			printf("%s: command not found\n", tools->command->args[0]);
	}
	if (g_signal == S_SIGINT_CMD)
			tools->exit_status = F_NOT_FILE;
	g_signal = S_BASE;
}
