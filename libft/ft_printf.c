/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:43:41 by ampocchi          #+#    #+#             */
/*   Updated: 2025/02/05 17:19:08 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	init_struct(char const *format, t_data *p)
{
	p->type = '\0';
	p->format = (char *)format;
	p->size = 0;
	p->pos = 0;
}

static void	ft_check_type(t_data *p)
{
	p->pos++;
	if (p->format[p->pos] == 'c')
		ft_write_c(va_arg(p->arg, int), p);
	else if (p->format[p->pos] == '%')
		ft_write_c('%', p);
	else if (p->format[p->pos] == 's')
		ft_write_s(va_arg(p->arg, char *), p);
	else if (p->format[p->pos] == 'd' || p->format[p->pos] == 'i')
		ft_write_d_i(va_arg(p->arg, int), p);
	else if (p->format[p->pos] == 'x')
		ft_write_x(va_arg(p->arg, unsigned int), 1, p);
	else if (p->format[p->pos] == 'X')
		ft_write_x(va_arg(p->arg, unsigned int), 0, p);
	else if (p->format[p->pos] == 'u')
		ft_write_u(va_arg(p->arg, unsigned int), p);
	else if (p->format[p->pos] == 'p')
		ft_write_p(va_arg(p->arg, void *), p);
	else
		return ;
}

int	ft_printf(char const *format, ...)
{
	t_data	p;
	int		totsize;

	va_start(p.arg, format);
	init_struct(format, &p);
	totsize = 0;
	while (p.format[p.pos])
	{
		if (p.format[p.pos] == '%' && p.format[p.pos] + 1)
		{
			ft_check_type(&p);
			totsize += p.size;
		}
		else
		{
			ft_putchar_fd(p.format[p.pos], 1);
			totsize++;
		}
		p.pos++;
	}
	va_end(p.arg);
	return (totsize);
}
