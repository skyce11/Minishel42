/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:58:46 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 15:16:48 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_right_str(char *str)
{
	char	*res;
	int		pos;
	size_t	lon;
	size_t	i;

	pos = get_char_pos(str, '=');
	if (pos == -1 || (size_t)pos == ft_strlen(str) - 1)
		return (NULL);
	lon = ft_strlen(str) - (size_t)pos - 1;
	res = malloc((lon + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < lon)
	{
		res[i] = str[(size_t)pos + 1 + i];
		i++;
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
