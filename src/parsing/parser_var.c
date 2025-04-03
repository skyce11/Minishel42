/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:58:46 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 11:18:15 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_right_str(char *str)
{
	char	*res;
	int		i;
	int		pos;
	int		lon_str;
	int		lon;
	int		j;

	pos = get_char_pos(str, '=');
	lon_str = ft_strlen(str);
	lon = ft_strlen(str) - pos;
	i = pos + 1;
	j = 0;
	res = ft_calloc(lon, sizeof(char));
	while (i < lon_str)
	{
		res[j] = str[i];
		i++;
		j++;
	}
	res[i] = '\0';
	return (res);
}

char	*get_left_str(char *str)
{
	char	*res;
	int		i;
	int		pos;

	pos = get_char_pos(str, '=');
	res = ft_calloc(pos, sizeof(char));
	i = 0;
	while (i < pos)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int	get_char_pos(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (0);
}

int	if_var(char *str)
{
	char	*aux;
	int		len;

	len = ft_strlen(str);
	aux = ft_strchr(str, '=');
	if (aux != NULL && (aux == str || str[0] != '\"' || str[len - 1] != '\"'))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int	if_ass_str(char **table)
{
	int	i;

	i = 0;
	while (table[i])
	{
		if (if_var(table[i]))
			i++;
		else
			return (0);
	}
	return (1);
}
