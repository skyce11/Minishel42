/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:29:41 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/07 13:11:39 by ampocchi         ###   ########.fr       */
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
	new_command->next = NULL;
	new_command->args = NULL;
	new_command->redir = NULL;
	new_command->return_val = 0;
	return (new_command);
}

/// @brief Frees the memory allocated for a command structure.
/// @param command Pointer to the command structure to be freed.
/// @return Void
void	free_command(t_command *command)
{
	if (command)
	{
		if (command->args)
			ft_free_arr(command->args);
		if (command->redir)
			cleanse_redir_list(command->redir);
		// if (command->cmd_sep)
			// free(command->cmd_sep);
		if (command->next)
			free_command(command->next);
		free(command);
	}
}
