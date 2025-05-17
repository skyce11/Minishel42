/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:09:43 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/17 13:17:37 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_env_name(const char *name)
{
	if (!name || !(*name))
		return (0);
	if (*name == '-' || !(ft_isalpha(*name) || *name == '_'))
		return (0);
	while (*name)
	{
		if (!(ft_isalnum(*name) || *name == '_'))
			return (0);
		name++;
	}
	return (1);
}

int	update_env_if_exists(t_tools *tools, char *name, char *value)
{
	int		i;
	char	*temp;

	i = 0;
	while (tools->envp[i])
	{
		if (ft_strncmp(tools->envp[i], name, ft_strlen(name)) == 0
			&& tools->envp[i][ft_strlen(name)] == '=')
		{
			free(tools->envp[i]);
			temp = ft_strjoin(name, "=");
			tools->envp[i] = ft_strjoin(temp, value);
			free(temp);
			free(name);
			free(value);
			return (1);
		}
		i++;
	}
	return (0);
}

void	printf_not_valid_env(t_tools *tools, char *name, char *value)
{
	ft_putstr_fd("export ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
	free(name);
	free(value);
	tools->exit_status = 1;
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
	char	*temp;
	int		i;

	name = get_env_name(var);
	value = get_env_value(var);
	if (!is_valid_env_name(name))
	{
		printf_not_valid_env(tools, name, value);
		return ;
	}
	if (update_env_if_exists(tools, name, value))
		return ;
	i = 0;
	while (tools->envp[i])
		i++;
	tools->envp = realloc(tools->envp, (i + 2) * sizeof(char *));
	temp = ft_strjoin(name, "=");
	tools->envp[i] = ft_strjoin(temp, value);
	free(temp);
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
	int		i;
	char	*var;

	tools->exit_status = 0;
	if (command->args[1] == NULL)
	{
		i = -1;
		while (tools->envp[i++])
			ft_putendl_fd(tools->envp[i], 1);
		return ;
	}
	i = 1;
	while (command->args[i])
	{
		var = command->args[i];
		if (ft_strchr(var, '='))
			add_or_update_env(tools, var);
		// else
		// {
		// 	ft_putstr_fd("export: ignoring invalid argument: ", STDERR_FILENO);
		// 	ft_putendl_fd(var, STDERR_FILENO);
		// }
		i++;
	}
}
