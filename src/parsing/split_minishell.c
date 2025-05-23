/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 12:13:15 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/14 18:07:28 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_quoted_token(char *s, char z)
{
	char	quote;
	size_t	len;
	char	*token;

	quote = 0;
	len = 0;
	while (s[len])
	{
		if ((s[len] == z || s[len] == '\t') && !quote)
			break ;
		if ((s[len] == '\'' || s[len] == '\"'))
		{
			if (quote == 0)
				quote = s[len];
			else if (quote == s[len])
				quote = 0;
		}
		len++;
	}
	token = ft_substr(s, 0, len);
	return (token);
}

/// @brief Splits a string into tokens based on a delimiter.
/// Allocates memory for an array of substrings and removes unnecessary spaces.
/// If memory allocation fails or no tokens are found, returns NULL.
/// tokens = cmd
/// @param s The string to split.
/// @param z The delimiter used for splitting.
/// @return A dynamically allocated array of substrings.
char	**split_minishell(char *s, char z)
{
	char	**pp;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = count_tokens(s, z);
	if (len == 0)
		return (NULL);
	pp = ft_calloc(len + 1, sizeof(s));
	if (!pp)
		return (NULL);
	i = 0;
	while (*s && i < len)
	{
		while (*s && (*s == z || *s == '\t'))
			s++;
		pp[i] = extract_quoted_token(s, z);
		if (!pp[i])
			return (ft_free_matrix(pp));
		s += ft_strlen(pp[i]);
		i++;
	}
	pp[i] = NULL;
	return (pp);
}

/// @brief Counts the number of tokens in a string, using a delimiter.
/// Iterates through the string, skipping delimiter characters and counting
/// valid segments.
/// @param s The string to analyze.
/// @param z The delimiter used for splitting.
/// @return The number of detected tokens in the string.
size_t	count_tokens(char *s, char z)
{
	size_t	i;

	i = 0;
	while (s && *s)
	{
		while (s && *s && (*s == z || *s == '\t'))
			s++;
		if (*s)
			i++;
		s = cross_string(s, NULL, z);
	}
	return (i);
}

/// @brief Moves the pointer past a specific character sequence.
/// Handles quoted strings, tabs, and delimiters,
//  advancing the pointer accordingly.
/// Also updates the length of the processed segment if needed.
/// @param s The string to analyze.
/// @param i Pointer to store the length of the skipped segment.
/// @param z The delimiter used for processing.
/// @return The updated pointer position in the string.
char	*cross_string(char *s, size_t *i, char z)
{
	char	c;
	size_t	len;

	c = 0;
	len = 0;
	if (*s == '\'' || *s == '\t')
	{
		c = *s;
		len++;
	}
	if (*s && (*s == c || *s == z || *s == '\t'))
		s++;
	s = get_end_str(s, &len, z);
	if (i)
		*i = len;
	return (s);
}

/// @brief Finds the end of a substring while considering quotes and delimiters.
/// Advances through the string, handling quoted segments and stopping at a
/// delimiter. Updates the length of the processed segment.
/// @param s The string to analyze.
/// @param c The active quote character, if any.
/// @param len Pointer to the length counter.
/// @param count Flag indicating whether inside a quoted section.
/// @param z The delimiter to check.
/// @return The updated pointer position in the string.
char	*get_end_str(char *s, size_t *len, char z)
{
	char	c;
	int		count;

	if (!s || !len)
		return (NULL);
	c = 0;
	count = 0;
	while (*s)
	{
		if ((*s == z || *s == '\t') && !(count == 1 && (*s == z || *s == '\t')))
			break ;
		if (c != 0 && (*s == c))
		{
			count = !count;
			c = 0;
		}
		else if (c == 0 && (*s == '\'' || *s == '\"'))
		{
			c = *s;
			count = !count;
		}
		s++;
		(*len)++;
	}
	return (s);
}
