/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:09:02 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/13 18:50:05 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Identifies the type of redirection operator in a string.
/// Scans the input while handling quoted sections to correctly detect
/// '>', '>>', '<', and '<<'.
/// @param redir The string containing redirection symbols.
/// @param i Pointer to the current index in the string.
/// @return 0 for '<', 1 for '>', 2 for '<<', 3 for '>>'
int	locate_redir(char *redir, int *i)
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
			locate_redir_aux(redir, i, &type, &quote);
		}
		*i = *i + 1;
	}
	return (type);
}

int	process_unquoted_char(char *redir, int *i, char *quote)
{
	if (redir[*i] == '"' || redir[*i] == '\'')
		*quote = redir[*i];
	else if (redir[*i] == ' ' || redir[*i] == '\t'
		|| redir[*i] == '\n' || redir[*i] == '<' || redir[*i] == '>')
		return (0);
	return (1);
}

size_t	extract_redir_word(char *redir, int *i)
{
	char	quote;
	size_t	length;

	quote = 0;
	length = 0;
	while (redir[*i])
	{
		if (quote == '"' || quote == '\'')
		{
			if (redir[*i] == quote)
			{
				(*i)++;
				length++;
				break ;
			}
		}
		else
		{
			if (!process_unquoted_char(redir, i, &quote))
				break ;
		}
		length++;
		(*i)++;
	}
	return (length);
}

size_t	preprocess_redir_file(char *redir, int *i,
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
	length = extract_redir_word(redir, i);
	if (length == 0)
		*status = -1;
	return (length);
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
