/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:06:43 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 15:27:39 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief si el commando es : echo / cd / pwd / export / unset / env / exit
/// @param command
/// @return 1 si es, sino 0
int	is_builtin(t_command *command)
{
	if (command->args[0])
	{
		if (!ft_strcmp(command->args[0], "echo"))
			return (1);
		if (!ft_strcmp(command->args[0], "cd"))
			return (1);
		if (!ft_strcmp(command->args[0], "pwd"))
			return (1);
		if (!ft_strcmp(command->args[0], "export"))
			return (1);
		if (!ft_strcmp(command->args[0], "unset"))
			return (1);
		if (!ft_strcmp(command->args[0], "env"))
			return (1);
		if (!ft_strcmp(command->args[0], "exit"))
			return (1);
		else
			return (0);
	}
	else
		return (0);
}

/// @brief redirige cada commando builtin a su propia funcion para ejecutarlo.
/// @param command
/// @param tools
/// @return void.
void	ft_builtin(t_command *command, t_tools *tools)
{
	if (!ft_strcmp(command->args[0], "env"))
		ft_env(command, tools);
	if (!ft_strcmp(command->args[0], "pwd"))
		ft_pwd(tools);
	if (!ft_strcmp(command->args[0], "echo"))
		ft_echo(command);
	if (!ft_strcmp(command->args[0], "exit"))
		ft_exit(tools);
	if (!ft_strcmp(command->args[0], "export"))
		ft_export(command, tools);
	if (!ft_strcmp(command->args[0], "unset"))
		ft_unset(command, tools);
	if (!ft_strcmp(command->args[0], "cd"))
		ft_cd(command, tools);
}
