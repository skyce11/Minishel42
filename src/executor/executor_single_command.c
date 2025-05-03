/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_single_command.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:34:38 by sperez-s          #+#    #+#             */
/*   Updated: 2025/04/22 12:05:03 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Ejecuta un comando incrustado en el proceso padre.
/// Guarda los descriptores estándar, ejecuta el comando,
/// luego restaura los descriptores para preservar la integridad del shell.
static void	run_builtin_in_parent_process(t_command *command, t_tools *tools)
{
	int	stdin_fd;
	int	stdout_fd;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	run_command(command, tools);
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}

// si el commando no es integrada, lanza la  ejecucion con un fork
// Gestiona las señales y actualiza el estado de ejecución una vez ejecutado el comando.
static int	fork_single_command(t_command *command, t_tools *tools)
{
	int	pid;
	int	status;

	if (fill_command_from_env(command, tools) == -1)
		return (-1);
	g_signal = S_CMD;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, sigquit_handler);
		run_command(command, tools);
		return (-1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		handle_status(status, tools);
	}
	else
		return (-1);
	g_signal = S_BASE;
	signal(SIGQUIT, SIG_IGN);
	return (status);
}

/// @brief La función `exec_single_command` es responsable de ejecutar
/// un único comando en el shell. Diferencia entre builtins
/// (builtins) y comandos externos, eligiendo un tratamiento apropiado
/// para cada uno.
/// @param command Estructura que contiene los detalles del comando.
/// @param tools Estructura que contiene las herramientas y la información
/// necesaria para la ejecución.
/// @return El estado de ejecución (0 para un comando incrustado o el resultado
/// del resultado de la bifurcación para un comando externo).
int	exec_single_command(t_command *command, t_tools *tools)
{
	int	status;

	if (is_builtin(command))
	{
		// ejecuta el commando directamente en el processo padre.
		run_builtin_in_parent_process(command, tools);
		// return 0, porque los commandos no necessitan un fork
		return (0);
	}
	status = fork_single_command(command, tools);
	return (status);
}
