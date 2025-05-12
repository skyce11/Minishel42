/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:11:08 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/12 14:31:46 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_err_exit(t_tools *tools, int err)
{
	if (err == 1)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		tools->exit_status = 1;
		return (1);
	}
	if (err == 2)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(tools->command->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (2);
	}
	return (0);
}

/// @brief Validates the arguments provided to the 'exit' command. Checks if
/// there are too many arguments, ensuring only one numeric value is allowed.
/// If a non-numeric argument is found, an error message is displayed.
/// @param tools Structure containing command details.
/// @return Exit status code
/// (0 by default, others if valid, 2 if invalid input).
static int	check_exit_args(t_tools *tools)
{
	int	i;
	int	nbr;

	nbr = 0;
	if (tools->command->args[1] && tools->command->args[2])
		return (print_err_exit(tools, 1));
	if (tools->command->args[1])
	{
		i = 0;
		if (tools->command->args[1][i] == '+'
			|| tools->command->args[1][i] == '-')
			i++;
		while (tools->command->args[1][i++])
		{
			if (!ft_isdigit(tools->command->args[1][i]))
				return (print_err_exit(tools, 2));
		}
		nbr = ft_atoi(tools->command->args[1]);
		if (nbr < 0)
			nbr += 256;
		else if (nbr > 256)
			nbr -= 256;
		return (nbr);
	}
	return (0);
}

/// @brief Comprueba los argumentos suministrados al comando, muestra un mensaje
/// de salida y libera los recursos.
/// @param tools
/// @return EXIT_FAILURE si el comando tiene demasiados argumentos,
/// sino solo exit el shell.
void	ft_exit(t_tools *tools)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	tools->exit_status = check_exit_args(tools);
	ft_clean_all(tools);
	exit(tools->exit_status);
}
