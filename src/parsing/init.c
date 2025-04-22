/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:29:41 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/22 11:02:17 by ampocchi         ###   ########.fr       */
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

/// @brief Frees the memory allocated for a command structure.
/// @param command Pointer to the command structure to be freed.
/// @return Void
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
