/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_minishell_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:54:10 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/14 18:07:33 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Calculates the length of a substring until a delimiter. Uses
/// `cross_string` to find the endpoint of the substring and returns its length.
/// @param s The string to analyze.
/// @param z The delimiter character.
/// @return The length of the substring before the delimiter.
size_t	count_string(char *s, char z)
{
	size_t	i;

	i = 0;
	cross_string(s, &i, z);
	return (i);
}

/// @brief Frees a dynamically allocated matrix (array of strings).
/// @param mtx The matrix (array of strings) to free.
/// @return Always returns NULL after freeing memory.
void	*ft_free_matrix(char **mtx)
{
	int	i;

	if (!mtx)
		return (NULL);
	i = 0;
	while (mtx[i])
	{
		free(mtx[i]);
		i++;
	}
	free(mtx);
	return (NULL);
}
