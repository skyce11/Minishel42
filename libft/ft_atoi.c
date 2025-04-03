/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 10:29:52 by migonzal          #+#    #+#             */
/*   Updated: 2021/10/04 09:16:37 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static	int	ft_is_space(int i)
{
	if ((i == 32) || (i == ' ') || (i == '\f')
		|| (i == '\n') || (i == '\r')
		|| (i == '\t') || i == '\v')
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int					i;
	int					s;
	unsigned long long	res;

	i = 0;
	s = 1;
	res = 0;
	while (ft_is_space(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+' )
	{
		if (str[i] == '-')
			s *= -1;
		i++;
	}
	while (str[i] != '\0' && ft_isdigit(str[i]))
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	if (res > 9223372036854775807 && s == -1)
		return (0);
	else if (res > 9223372036854775807)
		return (-1);
	return (res * s);
}
