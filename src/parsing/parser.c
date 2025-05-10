/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 11:04:58 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 15:16:47 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Parses a cmd string into separate commands using pipes as delimiters.
/// Splits the input string on '|' and stores each resulting command
/// in a linked structure.
/// @param s The string containing the command.
/// @return A pointer to the first command in the parsed sequence.
void	parser(t_tools *tools)
{
	char		**aux;
	t_command	*sep;
	int			i;

	aux = split_minishell(tools->arg_str, '|');
	if (!aux)
		return ;
	sep = NULL;
	i = 0;
	while (aux[i])
	{
		sep = add_cell(sep, aux[i]);
		if (!sep)
		{
			ft_free_arr(aux);
			return ;
		}
		i++;
	}
	ft_free_arr(aux);
	if (tools->command)
		free_command(tools->command);
	tools->command = sep;
}