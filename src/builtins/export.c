/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:09:43 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/11 14:09:28 by migonzal         ###   ########.fr       */
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
		fprintf(stderr, "export: '%s': not a valid identifier\n", name);
		free(name);
		free(value);
		tools->exit_status = 1;
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
