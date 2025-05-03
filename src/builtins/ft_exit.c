/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:11:08 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/03 19:03:22 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_tools(t_tools *tools)
{
	ft_free_arr(tools->paths);
	ft_free_arr(tools->envp);
	free(tools->arg_str);
	free(tools->pwd);
	free(tools->old_pwd);
}

/// @brief Validates the arguments provided to the 'exit' command. Checks if
/// there are too many arguments, ensuring only one numeric value is allowed.
/// If a non-numeric argument is found, an error message is displayed.
/// @param tools Structure containing command details.
/// @return Exit status code
/// (0 by default, others if valid, 255 if invalid input).
static int	check_exit_args(t_tools *tools)
{
	int	i;

	if (tools->command->args[1] && tools->command->args[2])
		return (1);
	if (tools->command->args[1])
	{
		i = 0;
		while (tools->command->args[1][i])
		{
			if (!ft_isdigit(tools->command->args[1][i]))
			{
				ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
				ft_putstr_fd(tools->command->args[1], STDERR_FILENO);
				ft_putendl_fd(": numeric argument required", STDERR_FILENO);
				return (255);
			}
			i++;
		}
		return (ft_atoi(tools->command->args[1]));
	}
	return (0);
}

/// @brief Comprueba los argumentos suministrados al comando, muestra un mensaje
/// de salida y libera los recursos.
/// @param tools
/// @return EXIT_FAILURE si el comando tiene demasiados argumentos,
/// sino solo exit el shell.
int	ft_exit(t_tools *tools)
{
	int	exit_code;

	ft_putendl_fd("exit", STDERR_FILENO);
	exit_code = check_exit_args(tools);
	if (exit_code == 1)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	free_tools(tools);
	exit(exit_code);
}
