/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:59:56 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/14 12:21:00 by ampocchi         ###   ########.fr       */
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

/// @brief Removes external quotes while respecting nested quotes.
/// For example, '"hello"' will become hello, and '"ho\'la"' will remain ho'la.
/// @param str The input string to process.
/// @return A newly allocated string with external quotes removed.
char	*remove_external_quotes(const char *str)
{
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	char	*result;

	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	result = malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/// @brief Finds a matching quote while respecting nested quotes.
/// @param line The input string.
/// @param i Current index in the string.
/// @param num_del Pointer to a counter for matched quotes.
/// @param del The delimiter quote character.
/// @return The distance to the matching quote or 0 if not found.
int	find_match_quote(char *line, int i, int *num_del, int del)
{
	int	j;

	j = i + 1;
	*num_del += 1;
	while (line[j])
	{
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
	return (0);
}

/// @brief Counts quotes in a line and ensures they are properly matched.
/// @param line The input string to analyze.
/// @return 1 if quotes are properly matched, 0 otherwise.
int	count_quotes(char *line)
{
	int	i;
	int	s;
	int	d;

	s = 0;
	d = 0;
	i = -1;
	while (line[++i])
	{
		if (line[i] == 34)
			i += find_match_quote(line, i, &d, 34);
		if (line[i] == 39)
			i += find_match_quote(line, i, &s, 39);
	}
	if ((d > 0 && d % 2 != 0) || (s > 0 && s % 2 != 0))
		return (0);
	return (1);
}
