/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:09:43 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 11:23:01 by migonzal         ###   ########.fr       */
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
