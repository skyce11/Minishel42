/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 10:37:36 by migonzal          #+#    #+#             */
/*   Updated: 2021/10/02 10:06:49 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*aux;
	size_t	len;

	len = ft_strlen(s1) + 1;
	aux = malloc (sizeof(char) * len);
	if (!aux)
		return (0);
	aux = ft_memcpy(aux, s1, len);
	return (aux);
}
