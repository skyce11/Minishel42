/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:52:12 by ampocchi          #+#    #+#             */
/*   Updated: 2025/02/05 18:07:03 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H

# define FT_PRINTF_H

# include "libft.h"
# include <stdio.h>
# include <stdarg.h>
# include <unistd.h>
# include <string.h>

# define BASE_10	"0123456789"
# define BASE_16	"0123456789abcdef"

typedef struct s_data
{
	char	type;
	char	*format;
	int		size;
	int		pos;
	va_list	arg;
}		t_data;

int		ft_printf(char const *format, ...);
void	ft_write_c(char c, t_data *p);
void	ft_write_s(char *str, t_data *p);
void	ft_write_d_i(int nb, t_data *p);
void	ft_write_x(unsigned int nb, int is_lowercase, t_data *p);
void	ft_write_u(unsigned int nb, t_data *p);
void	ft_write_p(void *adr, t_data *p);
char	*ft_itoa_unsigned_base(unsigned long nbr, char *base);
int		ft_count_digits(int nb, char *base);
int		ft_count_unsigned_digits(unsigned long nb, char *base);

#endif