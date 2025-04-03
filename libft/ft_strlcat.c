/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 10:38:44 by migonzal          #+#    #+#             */
/*   Updated: 2021/10/02 10:04:31 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	c;
	size_t	d;
	char	*aux;

	aux = (char *) src;
	if (dstsize <= ft_strlen(dst))
		return (dstsize + ft_strlen((aux)));
	c = ft_strlen(dst);
	d = 0;
	while (aux[d] != '\0' && c + 1 < dstsize)
	{
		dst[c] = aux[d];
		c++;
		d++;
	}
	dst[c] = '\0';
	return (ft_strlen(dst) + ft_strlen(&aux[d]));
}
