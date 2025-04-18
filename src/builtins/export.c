/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:09:43 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/18 14:17:48 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env(t_tools *tools)
{
	int	i;

	i = 0;
	while (tools->envp[i])
	{
		printf("%s\n", tools->envp[i]);
		i++;
	}
}

/// @brief Añade una nueva variable de entorno o actualiza una existente
/// existente en `tools->envp`. Si la variable ya existe,
/// es reemplazada por el nuevo valor. Si no existe, se añade.
/// @param tools
/// @param var Cadena que representa la variable de entorno.
void	add_or_update_env(t_tools *tools, char *var)
{
	char	*name;
	char	*value;
	int		i;

	name = get_env_name(var);
	value = get_env_value(var);
	i = 0;
	while (tools->envp[i])
	{
		if (ft_strncmp(tools->envp[i], name, ft_strlen(name))
			== 0 && tools->envp[i][ft_strlen(name)] == '=')
		{
			free(tools->envp[i]);
			tools->envp[i] = ft_strjoin(name, "=");
			tools->envp[i] = ft_strjoin(tools->envp[i], value);
			free(name);
			free(value);
			return ;
		}
		i++;
	}
	tools->envp = realloc(tools->envp, (i + 2) * sizeof(char *));
	tools->envp[i] = ft_strjoin(name, "=");
	tools->envp[i] = ft_strjoin(tools->envp[i], value);
	tools->envp[i + 1] = NULL;
	free(name);
	free(value);
}

/// @brief Usado para mostrar variables o añadir/editar variables existentes.
/// Modifica las variables de entorno directamente en `tools`.
/// @param command
/// @param tools
/// @return void.
void	ft_export(t_command *command, t_tools *tools)
{
	int	i;

	tools->exit_status = 0;
	if (command->args[1] == NULL)
	{
		print_env(tools);
	}
	else
	{
		i = 1;
		while (command->args[i])
		{
			add_or_update_env(tools, command->args[i]);
			i++;
		}
	}
}
