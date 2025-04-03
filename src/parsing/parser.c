/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 11:04:58 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 11:09:30 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		i++;
	}
	return (sep);
}
