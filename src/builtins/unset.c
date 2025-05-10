/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:11:30 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/08 17:51:42 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Borra una variable de entorno especificada por su nombre en
/// `tools->envp`. La función reorganiza el array `envp` para excluir la
/// variable y liberar memoria.
/// @param tools
/// @param name Nombre de la variable a eliminar.
static void	unset_variable(t_tools *tools, char *name)
{
	int		i;
	int		j;
	char	**new_envp;

	i = 0;
	j = 0;
	while (tools->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * i);
	if (!new_envp)
		return ;
	i = 0;
	while (tools->envp[i])
	{
		if (!(ft_strncmp(tools->envp[i], name, ft_strlen(name)) == 0
				&& tools->envp[i][ft_strlen(name)] == '='))
			new_envp[j++] = tools->envp[i];
		else
			free(tools->envp[i]);
		i++;
	}
	new_envp[j] = NULL;
	free(tools->envp);
	tools->envp = new_envp;
}

/// @brief borra una o más variables de entorno. Si no hay argumentos, el
/// comando no hace nada. Actualiza directamente el entorno en `tools` y ajusta
/// `tools->exit_status` para indicar el estado de ejecución.
/// @param command Estructura que contiene los argumentos del comando `unset`.
/// @param tools
/// @return void
void	ft_unset(t_command *command, t_tools *tools)
{
	int	i;

	i = 1;
	if (!command->args[i])
		return ;
	tools->exit_status = 0;
	while (command->args[i])
	{
		unset_variable(tools, command->args[i]);
		i++;
	}
}
