/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_csdi.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 14:45:32 by ampocchi          #+#    #+#             */
/*   Updated: 2025/02/05 18:06:25 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_count_digits(int nb, char *base)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
		return (11);
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		count++;
		nb = -nb;
	}
	while (nb > 0)
	{
		nb /= ft_strlen(base);
		count++;
	}
	return (count);
}

void	ft_write_c(char c, t_data *p)
{
	ft_putchar_fd(c, 1);
	p->size = 1;
}

void	ft_write_s(char *str, t_data *p)
{
	if (!str)
		str = "(null)";
	ft_putstr_fd(str, 1);
	p->size = ft_strlen(str);
}

void	ft_write_d_i(int nb, t_data *p)
{
	int	len;

	ft_putnbr_fd(nb, 1);
	len = ft_count_digits(nb, BASE_10);
	p->size = len;
}
