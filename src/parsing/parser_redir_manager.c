/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_manager.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:52:53 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/13 18:52:54 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	locate_redir_aux(char *redir, int *i, int *type, char *quote)
{
	if (redir[*i] == '"' || redir[*i] == '\'')
		*quote = redir[*i];
	else
	{
		if (redir[*i] == '>' && redir[*i + 1] && redir[*i + 1] == '>')
		{
			*i = *i + 1;
			*type = 3;
		}
		else if (redir[*i] == '>')
			*type = 1;
		else if (redir[*i] == '<'
			&& redir[*i + 1] && redir[*i + 1] == '<')
		{
			*i = *i + 1;
			*type = 2;
		}
		else if (redir[*i] == '<')
			*type = 0;
	}
}

// Función principal: Extrae y valida el nombre del archivo
char	*locate_redir_file(char *redir, int *i, int *status)
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
