/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sperez-s <sperez-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:55:10 by sperez-s          #+#    #+#             */
/*   Updated: 2025/03/26 20:03:38 by sperez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanse_pipe_list(t_pipe **first)
{
	t_pipe	*current;
	t_pipe	*next;

	if (!first || !*first)
		return ;
	current = *first;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

static t_pipe	*init_pipe(t_pipe *prev, t_pipe **list)
{
	t_pipe	*pipe;

	pipe = malloc(sizeof(t_pipe));
	if (!pipe)
	{
		cleanse_pipe_list(list);
		return (NULL);
	}
	pipe->next = NULL;
	if (prev)
		prev->next = pipe;
	return (pipe);
}

t_pipe	*create_pipe_list(int size)
{
	t_pipe	*first;
	t_pipe	*current;
	int		i;

	first = NULL;
	current = NULL;
	i = 0;
	if (size > 0)
	{
		first = init_pipe(NULL, NULL);
		if (!first)
			return (NULL);
		current = first;
		while (i < size - 1)
		{
			current = init_pipe(current, &first);
			i++;
		}
	}
	return (first);
}
