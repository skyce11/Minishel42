/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:08:32 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/03 21:00:50 by ampocchi         ###   ########.fr       */
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
	int	skip_newline;

	i = 1;
	skip_newline = 0;
	while (command->args[i] && !ft_strncmp(command->args[i], "-n", 2))
	{
		skip_newline = 1;
		i++;
	}
	while (command->args[i])
	{
		ft_putstr_fd(command->args[i++], STDOUT_FILENO);
		if (command->args[i])
			ft_putchar_fd(32, STDOUT_FILENO);
	}
	if (!skip_newline)
		ft_putchar_fd(10, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
