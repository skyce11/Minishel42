/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:07:53 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/17 18:35:26 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_change_oldpwd_env(t_tools *tools)
{
	char	*to_send;
	char	*path;

	path = getcwd(NULL, 0);
	to_send = ft_strjoin("OLDPWD=", path);
	add_or_update_env(tools, to_send);
	ft_memfree(to_send);
	ft_memfree(path);
}

static void	ft_change_pwd_env(t_tools *tools)
{
	char	*to_send;
	char	*path;

	path = getcwd(NULL, 0);
	to_send = ft_strjoin("PWD=", path);
	add_or_update_env(tools, to_send);
	ft_memfree(to_send);
	ft_memfree(path);
}

void	ft_print_cd(int err, int fd, t_tools *tools, t_command *command)
{
	ft_putstr_fd("minishell: cd: ", fd);
	if (err == 1)
		ft_putendl_fd("too many arguments", fd);
	if (err == 2)
	{
		ft_putstr_fd(command->args[1], fd);
		ft_putendl_fd(" No such file or directory", fd);
	}
	tools->exit_status = 1;
}

/// @brief Maneja el comando `cd` para cambiar el directorio de trabajo.
/// Actualiza las variables de entorno `OLDPWD` y `PWD`, y maneja
/// errores cuando la ruta no es válida o se suministran demasiados argumentos.
/// @param command Estructura que contiene los argumentos del comando `cd`.
/// @param tools Estructura que contiene datos del shell y variables de entorno.
void	ft_cd(t_command *command, t_tools *tools)
{
	char	*path;

	path = NULL;
	tools->exit_status = 0;
	if (command->args[1] && command->args[2])
		return (ft_print_cd(1, STDOUT_FILENO, tools, command));
	ft_change_oldpwd_env(tools);
	if (command->args[1])
	{
		if (ft_strcmp(command->args[1], "~") == 0)
			path = ft_strdup(getenv("HOME"));
		else
			path = ft_strdup(command->args[1]);
	}
	else if (!path)
		path = ft_strdup(getenv("HOME"));
	if (chdir(path))
		ft_print_cd(2, STDERR_FILENO, tools, command);
	ft_memfree(path);
	ft_change_pwd_env(tools);
}
