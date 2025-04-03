/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 10:32:47 by migonzal          #+#    #+#             */
/*   Updated: 2021/10/02 11:36:07 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_lstadd_front(t_list **list, t_list *new)
{
	if (!new)
		return (0);
	if (!list)
	{
		*list = new;
		return (0);
	}
	new -> next = *list;
	*list = new;
	return (0);
}
