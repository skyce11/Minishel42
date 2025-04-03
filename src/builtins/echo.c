/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:08:32 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 10:13:48 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
