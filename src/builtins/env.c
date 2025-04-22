/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:08:56 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/22 12:06:03 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Muestra todas las variables de entorno o devuelve un error
/// si se suministran argumentos adicionales.
/// Cambia el valor de `tools->exit_status` para reflejar el resultado
/// de la ejecución.
/// @param command
/// @param tools
/// @return void.
void	ft_env(t_command *command, t_tools *tools)
{
	int	i;

	i = 0;
	if (command->args[1])
	{
		printf("env: %s: No such file or directory\n", command->args[1]);
		tools->exit_status = 127;
		return ;
	}
	while (tools->envp[i])
	{
		printf("%s\n", tools->envp[i]);
		i++;
	}
	tools->exit_status = 0;
}
