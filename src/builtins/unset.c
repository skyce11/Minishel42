/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:11:30 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 10:15:35 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_variable(t_tools *tools, char *name)
{
	int		i;
	int		j;
	int		len;
	char	**new_envp;

	i = 0;
	j = 0;
	len = ft_strlen(name);
	while (tools->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * i);
	if (!new_envp)
		return ;
	i = 0;
	while (tools->envp[i])
	{
		if (!(ft_strncmp(tools->envp[i], name, len)
				== 0 && tools->envp[i][len] == '='))
		{
			new_envp[j] = tools->envp[i];
			j++;
		}
		else
		{
			free(tools->envp[i]);
		}
		i++;
	}
	new_envp[j] = NULL;
	free(tools->envp);
	tools->envp = new_envp;
}

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
