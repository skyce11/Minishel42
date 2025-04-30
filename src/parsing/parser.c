/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 11:04:58 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/29 21:42:57 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Parses a cmd string into separate commands using pipes as delimiters.
/// Splits the input string on '|' and stores each resulting command
/// in a linked structure.
/// @param s The string containing the command.
/// @return A pointer to the first command in the parsed sequence.
t_command	*parser(char *s)
{
	char		**aux;
	t_command	*sep;
	int			i;

	aux = split_minishell(s, '|');
	sep = NULL;
	i = 0;
	while (aux[i])
	{
		sep = add_cell(sep, aux[i]);
		if (!sep)
			return (NULL);
		i++;
	}
	return (sep);
}
