/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:11:19 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/22 12:07:44 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief La función `ft_pwd` muestra la ruta actual (directorio de trabajo)
/// de la misma forma que un comando `pwd` en una shell.
/// Actualiza el `exit_status` para indicar una ejecución exitosa.
/// @param tools
/// @return void
void	ft_pwd(t_tools *tools)
{
	char	*pwd;

	tools->exit_status = 0;
	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, 1);
	ft_memfree(pwd);
}
