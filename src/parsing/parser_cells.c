/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cells.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 10:25:18 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 16:42:17 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

// Función auxiliar: Inicializa la celda y procesa sus argumentos
static t_command	*initialize_and_process_cell(char *cmd_sep)
{
	t_command	*cell;

	cell = malloc(sizeof(t_command));
	if (!cell)
		return (NULL);
	cell->next = NULL;
	cell->cmd_sep = ft_strdup(cmd_sep);
	if (!cell->cmd_sep)
		return (free(cell), NULL);
	if (is_redirection_only(cmd_sep))
	{
		cell->args = malloc(sizeof(char *));
		if (!cell->args)
			return (free(cell->cmd_sep), free(cell), NULL);
		cell->args[0] = NULL;
	}
	else
	{
		cell->args = parse_args(cmd_sep);
		if (!cell->args)
			return (free(cell->cmd_sep), free(cell), NULL);
	}
	return (cell);
}

// Función principal: Crea la celda y procesa sus redirecciones
t_command	*create_cell(char *cmd_sep)
{
	t_command	*cell;
	int			status;

	status = 0;
	cell = initialize_and_process_cell(cmd_sep);
	if (!cell)
		return (NULL);
	cell->redir = create_redir_list(cmd_sep, &status);
	if (status == -1)
	{
		free(cell->cmd_sep);
		if (cell->args)
			ft_free_matrix(cell->args);
		free(cell);
		return (NULL);
	}
	return (cell);
}
/// @brief Adds a new command cell to the end of a linked list.
/// Creates a new command cell and attaches it to the last element in the list.
/// If the list is empty, the new cell becomes the first element.
/// @param list The existing list of commands.
/// @param cmd_sep The command string to store in the new cell.
/// @return The updated command list with the new cell added.

t_command	*add_cell(t_command *list, char *cmd_sep)
{
	t_command	*cur;
	t_command	*cell;

	cell = create_cell(cmd_sep);
	if (!cell)
	{
		if (list != NULL)
			free_command(list);
		return (NULL);
	}
	if (list == NULL)
		return (cell);
	cur = list;
	while (cur->next)
		cur = cur->next;
	cur->next = cell;
	return (list);
}
