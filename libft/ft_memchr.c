/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 10:34:54 by migonzal          #+#    #+#             */
/*   Updated: 2021/10/01 12:05:23 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*aux;
	size_t			i;

	aux = (unsigned char *) s;
	i = 0;
	while (i < n)
	{
		if (aux[i] == (unsigned char) c)
			return (aux + i);
		i++;
	}
	return (0);
}
