/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cells.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 10:25:18 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/29 21:38:55 by ampocchi         ###   ########.fr       */
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

/// @brief Extracts the filename associated with a redirection. Skips leading
/// spaces/tabs/newlines, then identifies and extracts the filename. Handles
/// quoted filenames properly and ensures memory allocation is successful.
/// If an error occurs, it updates the status and returns NULL.
/// @param redir The string containing redirection.
/// @param i Pointer to the current index in the string.
/// @param status Pointer to track errors.
/// @return A dynamically allocated string with the filename, or NULL on failure.
static char	*locate_redir_file(char *redir, int *i, int *status)
{
	char	quote;
	char	*word_start;
	char	*file;
	size_t	length;

	quote = 0;
	word_start = NULL;
	length = 0;
	file = NULL;
	while (redir[*i] && (redir[*i] == ' '
			|| redir[*i] == '\t' || redir[*i] == '\n'))
		*i = *i + 1;
	if (redir[*i] && (redir[*i] == '\\'
			|| redir[*i] == '<' || redir[*i] == '>'))
	{
		*status = -1;
		return (NULL);
	}
	word_start = redir + *i;
	while (redir[*i] && word_start)
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
			{
				if (word_start == NULL)
					word_start = redir + *i;
				quote = redir[*i];
			}
			else
			{
				if (redir[*i] == ' ' || redir[*i] == '\t'
					|| redir[*i] == '\n' || redir[*i] == '<'
					|| redir[*i] == '>')
					break ;
			}
		}
		length++;
		*i = *i + 1;
	}
	if (length == 0)
	{
		*status = -1;
		return (NULL);
	}
	file = ft_calloc(1, sizeof(char) * length + 1);
	if (!file)
	{
		*status = -1;
		return (NULL);
	}
	if (ft_strlcpy(file, word_start, length + 1) < length)
	{
		free(file);
		*status = -1;
		return (NULL);
	}
	return (file);
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

/// @brief Parses a redirection string and creates a linked list of redirection
/// nodes. Iterates through the input string, identifying redirection types and
/// associated filenames. If an error occurs, the allocated redirection list
/// is cleaned up before returning NULL.
/// @param redir The redirection string to process.
/// @param status Pointer to a status flag that monitors errors.
/// @return A pointer to the first redirection node, or NULL on failure.
t_redir	*create_redir_list(char *redir, int *status)
{
	int		type;
	int		i;
	char	*file;
	t_redir	*curr;
	t_redir	*first;
	t_redir	*aux;

	curr = NULL;
	first = NULL;
	aux = NULL;
	file = NULL;
	i = 0;
	type = -1;
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
		aux = create_redir_node(file, type, status);
		if (!aux)
		{
			cleanse_redir_list(first);
			return (NULL);
		}
		if (!first)
		{
			first = aux;
			curr = aux;
		}
		else
		{
			curr->next = aux;
			curr = curr->next;
		}
	}
	return (first);
}

/// @brief Creates a new command cell and initializes its properties.
/// Allocates memory for a command structure, processes arguments and redirections,
/// and sets the command separator.
/// If an error occurs during redirection processing, it returns NULL.
/// @param cmd_sep The command string to store in the new cell.
t_command	*create_cell(char *cmd_sep)
{
	t_command	*cell;
	int			status;

	status = 0;
	cell = malloc(sizeof(t_command));
	if (!cell)
		return (NULL);
	cell -> next = NULL;
	cell -> args = parse_args(cmd_sep);
	cell -> redir = create_redir_list(cmd_sep, &status);
	cell -> cmd_sep = cmd_sep;
	if (status == -1)
	{
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

	cur = list;
	cell = create_cell(cmd_sep);
	if (!cell)
		return (NULL);
	if (list == NULL)
		return (cell);
	while (cur->next)
		cur = cur->next;
	cur->next = cell;
	return (list);
}
