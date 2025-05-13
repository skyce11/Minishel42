/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:08:32 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/12 14:37:07 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo_aux(t_command *command, int *skip_newline, int i)
{
	int	j;

	*skip_newline = 0;
	while (command->args[i])
	{
		if (command->args[i][0] == '-')
		{
			j = 1;
			while (command->args[i][j] == 'n' && command->args[i][j] != '\0')
				j++;
			if (command->args[i][j] == '\0') // Todos los caracteres son 'n'
			{
				*skip_newline = 1;
				i++;
				continue;
			}
		}
		break; // Si no es una bandera válida, salimos del bucle
	}
	return (i);
}

/// @brief Esta función muestra los argumentos pasados, con o sin salto de
/// línea dependiendo de la opción `-n`.
/// @param command
/// @return Siempre EXIT_SUCCESS.
int	ft_echo(t_command *command)
{
	int	i;
	int	skip_newline;

	i = ft_echo_aux(command, &skip_newline, 1);
	while (command->args[i])
	{
		ft_putstr_fd(command->args[i++], STDOUT_FILENO);
		if (command->args[i])
			ft_putchar_fd(32, STDOUT_FILENO); // Imprime espacio entre argumentos
	}
	if (!skip_newline)
		ft_putchar_fd(10, STDOUT_FILENO); // Salto de línea si no está desactivado
	return (EXIT_SUCCESS);
}