/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:08:09 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/09 20:40:39 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clean_all(t_tools *tools)
{
	if (tools->arg_str)
		free(tools->arg_str);
	if (tools->command)
		free_command(tools->command);
	if (tools->pwd)
		free(tools->pwd);
	if (tools->old_pwd)
		free(tools->old_pwd);
	if (tools->envp)
		ft_free_arr(tools->envp);
	if (tools->paths)
		ft_free_arr(tools->paths);
}

int	check_child_status(int status, int fd, char *result, t_tools *tools)
{
	int	sig;

	sig = -1;
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		close(fd);
		free(result);
		if (sig == SIGINT)
			tools->exit_status = 130;
		else
			tools->exit_status = 2;
		return (EXIT_SUCCESS);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
	{
		close(fd);
		free(result);
		tools->exit_status = WEXITSTATUS(status);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

/// @brief Actualiza el estado de salida después de ejecutar un comando.
/// Maneja códigos de retorno, errores de comando no encontrado e
/// interrupciones SIGINT.
/// @param status Código de retorno del comando ejecutado.
/// @param tools
void	handle_status(int status, t_tools *tools)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			tools->exit_status = 130;
		else
			tools->exit_status = 2;
	}
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
}
