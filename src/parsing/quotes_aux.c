/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:02:00 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/22 11:55:16 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	quotes_lenght(char *str)
{
	int		i;
	size_t	aux;

	i = 0;
	aux = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			aux++;
		}
		i++;
	}
	return (aux);
}

/// @brief Removes all occurrences of a specified quote character from a string.
/// Iterates through the string, shifting characters to eliminate
/// consecutive quotes.
/// @param str The string to modify.
/// @param c The quote character to remove.
/// @return The modified string with quotes removed.
char	*delete_quotes(char *str, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			j = 0;
			while (str[i + j] == c)
				j++;
			ft_strlcpy(&str[i], &str[i + j], ft_strlen(str) - i);
		}
		i++;
	}
	return (str);
}
