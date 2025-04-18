/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 13:25:07 by sperez-s          #+#    #+#             */
/*   Updated: 2025/04/18 14:36:42 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ejecuta el comando dependiendo de si es un builtin o no
void	run_command(t_command *command, t_tools *tools)
{
	if (redir_setup(command) == 0)
	{
		if (is_builtin(command))
			ft_builtin(command, tools);
		else
			execve(command->args[0], command->args, tools->envp);
	}
}

/// @brief Esta función `executor` se encarga de gestionar la ejecución de los
/// comandos introducidos por el usuario. Determina si un único comando o un
/// comando (con tuberías, etc.) debe ser ejecutado.
/// @param tools Estructura que contiene los datos del shell,
/// incluyendo la lista de comandos.
/// @return 0 en caso de error, o el resultado de la ejecución del comando.
int	executor(t_tools *tools)
{
	unsigned int	size; // numero total de commando

	size = get_command_list_size(tools->command);
	// verifica si la lista de commando esta vacia
	if (size < 1)
	{
		printf("ERROR: empty command list\n");
		return (0);
	}
	// si solo hay un commando, se ejecuta directamente
	if (size == 1)
		return (exec_single_command(tools->command, tools));
	return (exec_compound_command(tools, size));
}
