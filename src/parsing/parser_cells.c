/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cells.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 10:25:18 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/08 13:51:14 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

/// @brief Identifies the type of redirection operator in a string.
/// Scans the input while handling quoted sections to correctly detect
/// '>', '>>', '<', and '<<'.
/// @param redir The string containing redirection symbols.
/// @param i Pointer to the current index in the string.
/// @return 0 for '<', 1 for '>', 2 for '<<', 3 for '>>'
static int	locate_redir(char *redir, int *i)
{
	char	quote;
	int		type;

	quote = 0;
	type = -1;
	while (redir[*i] && type == -1)
	{
		if (quote == '"' || quote == '\'')
		{
			if (redir[*i] == quote)
				quote = 0;
		}
		else
		{
			if (redir[*i] == '"' || redir[*i] == '\'')
				quote = redir[*i];
			else
			{
				if (redir[*i] == '>' && redir[*i + 1] && redir[*i + 1] == '>')
				{
					*i = *i + 1;
					type = 3;
				}
				else if (redir[*i] == '>')
					type = 1;
				else if (redir[*i] == '<'
					&& redir[*i + 1] && redir[*i + 1] == '<')
				{
					*i = *i + 1;
					type = 2;
				}
				else if (redir[*i] == '<')
					type = 0;
			}
		}
		*i = *i + 1;
	}
	return (type);
}

static size_t	preprocess_redir_file(char *redir, int *i,
			int *status, char **word_start)
{
	char	quote;
	size_t	length;

	quote = 0;
	length = 0;
	while (redir[*i] && (redir[*i] == ' '
			|| redir[*i] == '\t' || redir[*i] == '\n'))
		*i = *i + 1;
	if (redir[*i] && (redir[*i] == '\\'
			|| redir[*i] == '<' || redir[*i] == '>'))
	{
		*status = -1;
		return (0);
	}
	*word_start = redir + *i;
	while (redir[*i] && *word_start)
	{
		if (quote == '"' || quote == '\'')
		{
			if (redir[*i] == quote)
			{
				*i = *i + 1;
				length++;
				break ;
			}
		}
		else
		{
			if (redir[*i] == '"' || redir[*i] == '\'')
				quote = redir[*i];
			else if (redir[*i] == ' ' || redir[*i] == '\t' || redir[*i] == '\n'
				|| redir[*i] == '<' || redir[*i] == '>')
				break ;
		}
		length++;
		*i = *i + 1;
	}
	if (length == 0)
		*status = -1;
	return (length);
}

// Función principal: Extrae y valida el nombre del archivo
static char	*locate_redir_file(char *redir, int *i, int *status)
{
	char	*file;
	char	*word_start;
	size_t	length;

	word_start = NULL;
	length = preprocess_redir_file(redir, i, status, &word_start);
	if (*status == -1 || length == 0)
		return (NULL);
	file = ft_calloc(1, sizeof(char) * (length + 1));
	if (!file || ft_strlcpy(file, word_start, length + 1) < length)
	{
		free(file);
		*status = -1;
		return (NULL);
	}
	return (file);
}

static int	is_redirection_only(char *cmd_sep)
{
	int	i;

	i = 0;
	while (cmd_sep[i])
	{
		if (cmd_sep[i] != ' ' && cmd_sep[i] != '\t' && cmd_sep[i] != '\n'
			&& cmd_sep[i] != '<' && cmd_sep[i] != '>' && cmd_sep[i] != '|')
		{
			return (0);
		}
		i++;
	}
	return (1);
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

// Función auxiliar: Crea un nodo de redirección y lo agrega a la lista
static int	add_redir_node(t_redir **list, char *file, int type, int *status)
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

// Función principal refactorizada: Crea la lista de redirecciones
t_redir	*create_redir_list(char *redir, int *status)
{
	int		type;
	int		i;
	char	*file;
	t_redir	*first;

	first = NULL;
	i = 0;
	while (redir[i])
	{
		type = locate_redir(redir, &i);
		if (type == -1)
			return (first);
		file = locate_redir_file(redir, &i, status);
		if (!file)
		{
			cleanse_redir_list(first);
			return (NULL);
		}
		if (add_redir_node(&first, file, type, status) == -1)
			return (NULL);
	}
	return (first);
}

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
	{
		free(cell);
		return (NULL);
	}
	if (is_redirection_only(cmd_sep))
	{
		cell->args = malloc(sizeof(char *));
		if (!cell->args)
		{
			free(cell->cmd_sep);
			free(cell);
			return (NULL);
		}
		cell->args[0] = NULL;
	}
	else
	{
		cell->args = parse_args(cmd_sep);
		if (!cell->args)
		{
			free(cell->cmd_sep);
			free(cell);
			return (NULL);
		}
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
