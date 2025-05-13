/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:27:37 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/13 18:38:22 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// get list size
int	get_command_list_size(t_command *list)
{
	int	i;

	i = 0;
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

/// @brief get a pipe from the list based on its position and whether it is the
/// previous pipe or not.
/// It traverses the linked list of pipes to find the relevant one.
/// @param ps Pointer to the head of the pipe list.
/// @param pos Position of the pipe in the sequence.
/// @param is_prev Indicates whether the requested pipe is the previous one (1)
/// or the current one (0).
/// @return Pointer to the requested pipe, or NULL if the previous pipe
t_pipe	*obtain_related_pipe_from_list(t_pipe *ps,
		unsigned int pos, int is_prev)
{
	t_pipe			*curr_pipe;
	unsigned int	i;

	curr_pipe = ps;
	i = 1;
	if (!is_prev)
		i = 0;
	while (i < pos && curr_pipe)
	{
		curr_pipe = curr_pipe->next;
		i++;
	}
	if (pos == 0 && is_prev)
		return (NULL);
	return (curr_pipe);
}
