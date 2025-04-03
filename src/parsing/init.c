/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:29:41 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 10:44:56 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_command(void)
{
	t_command	*new_command;

	new_command = (t_command *) malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	new_command->cmd_sep = NULL;
	new_command->args = NULL;
	new_command->return_val = 0;
	new_command->next = NULL;
	return (new_command);
}

void	free_command(t_command *command)
{
	char	**args;

	if (command)
	{
		if (command->cmd_sep)
		{
			args = command->args;
			while (*args)
			{
				free(*args);
				args++;
			}
			free(command->args);
		}
		if (command->redir)
			free(command->redir);
		free(command);
	}
}
