/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:59:56 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 15:16:44 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Duplicate an array of strings with MALLOC
/// @param arr The original array of strings to be duplicated.
/// @return A newly allocated copy of the array of strings, or NULL on failure.
char	**arrdup(char **arr)
{
	char	**aux;
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	aux = ft_calloc(sizeof(char *), i + 1);
	if (!aux)
		return (NULL);
	i = 0;
	while (arr[i] != NULL)
	{
		aux[i] = ft_strdup(arr[i]);
		if (aux[i] == NULL)
		{
			ft_free_arr(aux);
			return (aux);
		}
		i++;
	}
	return (aux);
}

// size_t	pos_after_char(char *str, char c)
// {
// 	size_t	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == c)
// 			return (i + 1);
// 		i++;
// 	}
// 	return (0);
// }

// int	find_match_quote(char *line, int i, int *num_del, int del)
// {
// 	int	j;

// 	j = i + 1;
// 	*num_del += 1;
// 	while (line[j] && line[j] != del)
// 		j++;
// 	if (line[j] == del)
// 		*num_del += 1;
// 	else
// 		return (0);
// 	return (j - i);
// }

// int	count_quotes(char *line)
// {
// 	int	i;
// 	int	s;
// 	int	d;

// 	s = 0;
// 	d = 0;
// 	i = -1;
// 	while (line[++i])
// 	{
// 		if (line[i] == 34)
// 			i += find_match_quote(line, i, &d, 34);
// 		if (line[i] == 39)
// 			i += find_match_quote(line, i, &s, 39);
// 	}
// 	if ((d > 0 && d % 2 != 0) || (s > 0 && s % 2 != 0))
// 		return (0);
// 	return (1);
// }

/// @brief Removes external quotes while respecting nested quotes.
/// For example, '"hello"' will become hello, and '"ho\'la"' will remain ho'la.
/// @param str The input string to process.
/// @return A newly allocated string with external quotes removed.
char *remove_external_quotes(const char *str)
{
    int i = 0;
    int j = 0;
    int in_single_quote = 0; // Track if inside single quotes
    int in_double_quote = 0; // Track if inside double quotes
    char *result = malloc(strlen(str) + 1);

    if (!result)
        return NULL;

    while (str[i])
    {
        if (str[i] == '\'' && !in_double_quote)
        {
            // Toggle single quote flag
            in_single_quote = !in_single_quote;
        }
        else if (str[i] == '\"' && !in_single_quote)
        {
            // Toggle double quote flag
            in_double_quote = !in_double_quote;
        }
        else
        {
            // Copy character if not an external quote
            result[j++] = str[i];
        }
        i++;
    }
    result[j] = '\0'; // Null-terminate the result
    return result;
}

/// @brief Finds a matching quote while respecting nested quotes.
/// @param line The input string.
/// @param i Current index in the string.
/// @param num_del Pointer to a counter for matched quotes.
/// @param del The delimiter quote character.
/// @return The distance to the matching quote or 0 if not found.
int find_match_quote(char *line, int i, int *num_del, int del)
{
    int j;

    j = i + 1;
    *num_del += 1;
    while (line[j])
    {
        // Skip nested quotes of the opposite type
        if (line[j] == '\'' && del == '\"')
            j++;
        else if (line[j] == '\"' && del == '\'')
            j++;
        else if (line[j] == del)
        {
            *num_del += 1;
            return (j - i);
        }
        j++;
    }
    return (0); // No matching quote found
}

/// @brief Counts quotes in a line and ensures they are properly matched.
/// @param line The input string to analyze.
/// @return 1 if quotes are properly matched, 0 otherwise.
int count_quotes(char *line)
{
    int i;
    int s;
    int d;

    s = 0;
    d = 0;
    i = -1;
    while (line[++i])
    {
        if (line[i] == 34) // Double quote (")
            i += find_match_quote(line, i, &d, 34);
        if (line[i] == 39) // Single quote (')
            i += find_match_quote(line, i, &s, 39);
    }
    if ((d > 0 && d % 2 != 0) || (s > 0 && s % 2 != 0))
        return (0);
    return (1);
}

