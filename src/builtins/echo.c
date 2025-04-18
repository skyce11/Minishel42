/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:08:32 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/17 22:24:54 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Esta función muestra los argumentos pasados, con o sin salto de
/// línea dependiendo de la opción `-n`.
/// @param command
/// @return Siempre EXIT_SUCCESS.
int	ft_echo(t_command *command)
{
	int	i;

	i = 1;
	if (command->args[1] && !ft_strncmp(command->args[1], "-n", 2))
		i++;
	while (command->args[i])
	{
		ft_putstr_fd(command->args[i++], STDOUT_FILENO);
		if (command->args[i])
			ft_putchar_fd(32, STDOUT_FILENO);
	}
	if (!(command->args[1] && !ft_strncmp(command->args[1], "-n", 2)))
		ft_putchar_fd(10, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
