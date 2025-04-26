/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:44:38 by ampocchi          #+#    #+#             */
/*   Updated: 2025/02/05 18:04:36 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin_gnl(int bytes_read, char *s1, char *s2)
{
	int		total_size;
	char	*dest;

	if (!s1 || !s2)
		return (NULL);
	s2[bytes_read] = '\0';
	total_size = ft_strlen(s1) + ft_strlen(s2);
	dest = malloc(total_size + 1);
	if (!dest)
		return (NULL);
	dest[0] = '\0';
	dest = ft_strcat(dest, s1);
	dest = ft_strcat(dest, s2);
	free(s1);
	return (dest);
}

char	*ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
	size_t	len_s;
	size_t	i;
	char	*dest;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (start >= len_s)
		return (ft_strdup(""));
	if (len > len_s - start)
		len = len_s - start;
	dest = malloc(sizeof(char) * len + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[len] = '\0';
	return (dest);
}
