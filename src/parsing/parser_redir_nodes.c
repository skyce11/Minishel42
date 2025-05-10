/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_nodes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:08:51 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 15:17:04 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Función auxiliar: Crea un nodo de redirección y lo agrega a la lista
int	add_redir_node(t_redir **list, char *file, int type, int *status)
{
	t_redir	*aux;
	t_redir	*curr;

	aux = create_redir_node(file, type, status);
	if (!aux)
	{
		cleanse_redir_list(*list);
		return (-1);
	}
	if (!*list)
		*list = aux;
	else
	{
		curr = *list;
		while (curr->next)
			curr = curr->next;
		curr->next = aux;
	}
	return (0);
}

/// @brief Frees memory allocated for a list of redirection structures.
/// @param redir Pointer to the first redirection node.
/// @return Void
void	cleanse_redir_list(t_redir *redir)
{
	t_redir	*next;

	if (!redir)
		return ;
	while (redir)
	{
		next = redir;
		free(next->file);
		redir = next->next;
		free(next);
	}
}

/// @brief Creates a new redirection node.
/// Allocates memory for a redirection structure and initializes its properties.
/// If memory allocation fails, the function frees the file and returns NULL.
/// @param file The file associated with the redirection.
/// @param type The type of redirection (e.g., input, output, append).
/// @param status Pointer to track errors.
/// @return A pointer to the newly created redirection node, or NULL on failure.
t_redir	*create_redir_node(char *file, int type, int *status)
{
	t_redir	*node;

	node = NULL;
	node = malloc(sizeof(t_redir));
	if (!node)
	{
		free(file);
		*status = -1;
		return (NULL);
	}
	node->file = NULL;
	node->fd = -1;
	node->type = -1;
	node->next = NULL;
	node->file = file;
	node->type = type;
	return (node);
}
