/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:02:00 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 15:16:45 by migonzal         ###   ########.fr       */
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
// char	*delete_quotes(char *str, char c)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == c)
// 		{
// 			j = 0;
// 			while (str[i + j] == c)
// 				j++;
// 			ft_strlcpy(&str[i], &str[i + j], ft_strlen(str) - i);
// 		}
// 		i++;
// 	}
// 	return (str);
// }
