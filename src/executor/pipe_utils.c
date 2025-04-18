/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:55:10 by sperez-s          #+#    #+#             */
/*   Updated: 2025/04/18 14:42:59 by ampocchi         ###   ########.fr       */
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

/// @brief Inicializa una estructura `t_pipe` para manejar tuberías en una lista
/// encadenada. Si falla, limpia la lista existente antes de devolver `NULL`.
/// @param prev Puntero a la tubería anterior de la lista (o NULL para la primera tubería).
/// @param list Puntero a la cabecera de la lista de tuberías (para gestión en caso de fallo).
/// @return Puntero a la nueva estructura `t_pipe` inicializada, o `NULL` en caso de error.
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

/// @brief Crea una lista encadenada de estructuras `t_pipe` para gestionar las
/// tuberías necesarias en una serie de comandos compuestos.
/// @param size Número de tuberías a crear (vinculado al número de comandos - 1).
/// @return Puntero a la primera estructura `t_pipe` de la lista, sino `NULL`.
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
