/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:17:38 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/17 12:55:47 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief busca el '$' dentro de la cadena
/// @param str
/// @return el indice siguiente si lo ha econtrado. Sino, return 0
size_t	dollar_after(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (i + 1);
		i++;
	}
	return (0);
}

/// @brief Finds the position after the first '=' character in a string.
/// Scans the string for the '=' symbol and returns the index of the next char.
/// If '=' is not found, returns 0.
/// @param str The string to analyze.
/// @return The index after '=' or 0 if not found.
size_t	equal_after(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i +1);
		i++;
	}
	return (0);
}

/// @brief Converts a single character into a dynamically allocated string.
/// Allocates memory for a string with two characters: the character and '\0'.
/// @param c The character to convert.
/// @return A pointer to the newly allocated string.
char	*char_to_str(char c)
{
	char	*str;

	str = ft_calloc(sizeof(char), 2);
	str[0] = c;
	return (str);
}

/// @brief Checks if a digit follows a dollar sign in a string.
/// If the character at position `j` is '$' and the next character is a digit,
/// it advances the index by two positions.
/// @param j The current index position in the string.
/// @param str The string to analyze.
/// @return The number of positions the index has moved.
int	digit_after_dollar(int j, char *str)
{
	int	i;

	i = j;
	if (str[j] == '$')
	{
		if (ft_isdigit(str[j + 1]) == 1)
			j += 2;
	}
	return (j - i);
}

/// @brief Determines the length of a variable name after a '$' symbol.
/// Iterates through the string, stopping at special characters or the next '$'.
/// @param str The string to analyze.
/// @param j The starting index of '$'.
/// @return The index where the variable name ends.
int	after_dollar_lenght(char *str, int j)
{
	int	i;

	i = j + 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			break; // Detente si encuentras un carácter no válido
		i++;
	}
	return (i);
}
