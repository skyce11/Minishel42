/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:08:09 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/17 21:29:42 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Resets the structure containing execution tools.
/// Frees allocated memory, clears stored data, and reinitializes the structure.
/// @param tools Pointer to the tools structure being reset.
/// @return Always returns 1 after successful reset.
int	reset_tools(t_tools *tools)
{
	int	last_exit_status;

	last_exit_status = tools->exit_status;
	signal_init();
	free_command(tools->command);
	if (tools->arg_str)
		free(tools->arg_str);
	if (tools->paths)
		ft_free_arr(tools->paths);
	if (tools->pwd)
		free(tools->pwd);
	if (tools->old_pwd)
		free(tools->old_pwd);
	tools->paths = NULL;
	init_tools(tools);
	tools->exit_status = last_exit_status;
	tools->reset = 1;
	return (1);
}

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

/// @brief Actualiza el estado de salida después de ejecutar un comando.
/// Maneja códigos de retorno, errores de comando no encontrado e
/// interrupciones SIGINT.
/// @param status Código de retorno del comando ejecutado.
/// @param tools
void	handle_status(int status, t_tools *tools)
{
	if (WIFEXITED(status))
		tools->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		tools->exit_status = 128 + WTERMSIG(status);
}
