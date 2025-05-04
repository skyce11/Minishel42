/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 13:25:07 by sperez-s          #+#    #+#             */
/*   Updated: 2025/04/22 12:04:36 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Run the command depends if is a builton or not.
/// @param command The command to be executed.
/// @param tools
/// @return Void
// void	run_command(t_command *command, t_tools *tools)
// {
// 	if (redir_setup(command) == 0)
// 	{
// 		if (is_builtin(command))
// 			ft_builtin(command, tools);
// 		else
// 			execve(command->args[0], command->args, tools->envp);
// 	}
// }

void	run_command(t_command *command, t_tools *tools)
{
	if (redir_setup(command) == 0)
	{
		if (command->args == NULL || command->args[0] == NULL)
		{
			// Caso de redirección sin comando: no hacer nada más
			// porque el archivo ya se ha creado/configurado en redir_setup
			return;
		}
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
// int	executor(t_tools *tools)
// {
// 	write (1, "HOLA\n", 5);
// 	if (tools->command->args == NULL || tools->command->args[0] == NULL)
// 	{
// 		fprintf(stderr, "Error: args is NULL\n");
// 		return (-1);
// 	}
// 	write (1, "HOLO\n", 5);
// 	unsigned int	size; // numero total de commando

// 	size = get_command_list_size(tools->command);
// 	// verifica si la lista de commando esta vacia

// 	if (!tools->command->args && tools->command->redir)
// 	{
// 		if (redir_setup_no_command(tools->command->redir) == 0)
// 			return (0); // Éxito al manejar redirecciones sin comando
// 		return (-1);    // Error al manejar redirecciones
// 	}

// 	if (size < 1)
// 	{
// 		printf("ERROR: empty command list\n");
// 		return (0);
// 	}
// 	// si solo hay un commando, se ejecuta directamente
// 	if (size == 1)
// 		return (exec_single_command(tools->command, tools));
// 	return (exec_compound_command(tools, size));
// }


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
	{
		// Validar si `args[0]` es NULL y solo considerar redirecciones
		if (tools->command->args == NULL || tools->command->args[0] == NULL)
		{
			if (redir_setup(tools->command) == 0)
				return (1); // Redirección manejada correctamente
			else
				return (0); // Error al manejar la redirección
		}
		return (exec_single_command(tools->command, tools));
	}
	return (exec_compound_command(tools, size));
}