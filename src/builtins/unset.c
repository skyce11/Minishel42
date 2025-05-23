/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:11:30 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/18 11:39:19 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Borra una variable de entorno especificada por su nombre en
/// `tools->envp`. La función reorganiza el array `envp` para excluir la
/// variable y liberar memoria.
/// @param tools
/// @param name Nombre de la variable a eliminar.

void	copy_env(t_tools *tools, char *name, char	**new_envp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tools->envp[i])
	{
		if (!(ft_strncmp(tools->envp[i], name, ft_strlen(name)) == 0
				&& tools->envp[i][ft_strlen(name)] == '='))
			new_envp[j++] = strdup(tools->envp[i]);
		i++;
	}
	new_envp[j] = NULL;
}

static void	unset_variable(t_tools *tools, char *name)
{
	int		i;
	char	**new_envp;
	char	**old_envp;

	i = 0;
	if (!tools->envp)
		return ;
	while (tools->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
		return ;
	old_envp = tools->envp;
	copy_env(tools, name, new_envp);
	ft_free_matrix(old_envp);
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
