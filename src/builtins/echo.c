/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:08:32 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/11 16:03:14 by migonzal         ###   ########.fr       */
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
	int j;
	int z;
	int	skip_newline;

	i = 1;
	z = 1;
	skip_newline = 0;
	while (command->args[z])
	{
		if (command->args[i][0] == '-')
		{
			j = 1;
			while (command->args[i][j]  == 'n')
				j++;
			if (!command->args[i][j] || command->args[i][j] == 'n')
			{
				skip_newline = 1;
				i++;
			}
		}
		z++;
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
