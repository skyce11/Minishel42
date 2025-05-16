/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:08:09 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/16 17:28:08 by ampocchi         ###   ########.fr       */
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
		else if (sig == 127)
			tools->exit_status = 127;
		else
			tools->exit_status = 0;
	}
}
