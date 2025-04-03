/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 10:30:19 by migonzal          #+#    #+#             */
/*   Updated: 2021/10/02 10:09:45 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*pmemory;
	size_t	i;

	pmemory = malloc(count * size);
	if (pmemory != NULL)
	{
		i = 0;
		while (i < count * size)
		{
			pmemory[i] = 0;
			i++;
		}
	}
	return (pmemory);
}
