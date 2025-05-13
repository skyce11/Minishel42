/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:09:02 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/13 17:52:54 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	locate_redir_aux(char *redir, int *i, int *type, char *quote)
// {
// 	if (redir[*i] == '"' || redir[*i] == '\'')
// 		*quote = redir[*i];
// 	else
// 	{
// 		if (redir[*i] == '>' && redir[*i + 1] && redir[*i + 1] == '>')
// 		{
// 			*i = *i + 1;
// 			*type = 3;
// 		}
// 		else if (redir[*i] == '>')
// 			*type = 1;
// 		else if (redir[*i] == '<'
// 			&& redir[*i + 1] && redir[*i + 1] == '<')
// 		{
// 			*i = *i + 1;
// 			*type = 2;
// 		}
// 		else if (redir[*i] == '<')
// 			*type = 0;
// 	}
// }

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
			// locate_redir_aux(redir, i, &type, &quote);
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

// static int	process_unquoted_char(char *redir, int *i, char *quote)
// {
// 	if (redir[*i] == '"' || redir[*i] == '\'')
// 		*quote = redir[*i];
// 	else if (redir[*i] == ' ' || redir[*i] == '\t' ||
// 			redir[*i] == '\n' || redir[*i] == '<' || redir[*i] == '>')
// 		return (0);
// 	return (1);
// }

// static size_t	extract_redir_word(char *redir, int *i)
// {
// 	char	quote;
// 	size_t	length;

// 	quote = 0;
// 	length = 0;
// 	while (redir[*i])
// 	{
// 		if (quote == '"' || quote == '\'')
// 		{
// 			if (redir[*i] == quote)
// 			{
// 				(*i)++;
// 				length++;
// 				break ;
// 			}
// 		}
// 		else
// 		{
// 			if (!process_unquoted_char(redir, i, &quote))
// 				break;
// 		}
// 		length++;
// 		(*i)++;
// 	}
// 	return (length);
// }

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
		// length = extract_redir_word(redir, i);
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

int	is_redirection_only(char *cmd_sep)
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
