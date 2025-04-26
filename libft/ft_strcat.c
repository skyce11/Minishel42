/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:45:35 by ampocchi          #+#    #+#             */
/*   Updated: 2025/02/22 21:06:30 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Allocates (with malloc(3)) and returns a new string,
// which is the result of the concatenation of ’s1’ and ’s2’.

#include "libft.h"

char	*ft_strcat(char *dest, char const *str)
{
	int		len;
	int		i;

	len = ft_strlen(dest);
	i = 0;
	while (str[i])
	{
		dest[len + i] = str[i];
		i++;
	}
	dest[len + i] = '\0';
	return (dest);
}
