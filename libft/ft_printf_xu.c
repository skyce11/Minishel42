/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_xu.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 14:45:45 by ampocchi          #+#    #+#             */
/*   Updated: 2025/02/05 18:06:34 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_count_unsigned_digits(unsigned long nb, char *base)
{
	int	count;

	count = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb /= ft_strlen(base);
		count++;
	}
	return (count);
}

char	*ft_itoa_unsigned_base(unsigned long nbr, char *base)
{
	int		base_len;
	int		digit_count;
	char	*result;

	digit_count = ft_count_unsigned_digits(nbr, base);
	base_len = ft_strlen(base);
	result = (char *)malloc((digit_count + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[digit_count] = '\0';
	while (digit_count > 0)
	{
		result[--digit_count] = base[nbr % base_len];
		nbr /= base_len;
	}
	return (result);
}

void	ft_write_x(unsigned int nb, int is_lowercase, t_data *p)
{
	char	*result;
	char	*base;

	if (is_lowercase == 1)
		base = BASE_16;
	else
		base = "0123456789ABCDEF";
	result = ft_itoa_unsigned_base(nb, base);
	p->size = ft_strlen(result);
	ft_putstr_fd(result, 1);
	free(result);
}

void	ft_write_u(unsigned int nb, t_data *p)
{
	char	*result;

	result = ft_itoa_unsigned_base(nb, BASE_10);
	p->size = ft_strlen(result);
	ft_putstr_fd(result, 1);
	free(result);
}

void	ft_write_p(void *adr, t_data *p)
{
	unsigned long	ptr;
	char			*result;

	ptr = (unsigned long)adr;
	if (ptr == 0)
	{
		ft_putstr_fd("(nil)", 1);
		p->size = 5;
	}
	else
	{
		result = ft_itoa_unsigned_base(ptr, BASE_16);
		p->size = ft_strlen(result) + 2;
		ft_putstr_fd("0x", 1);
		ft_putstr_fd(result, 1);
		free(result);
	}
}
