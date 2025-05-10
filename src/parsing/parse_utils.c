/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:39:33 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 13:04:41 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_dup_after(char *s, char c)
{
	char	**pp;
	size_t	len;
	size_t	j;
	size_t	aux;

	j = 0;
	if (!s)
		return (NULL);
	len = count_tokens(s, c);
	if (len == 0)
		return (NULL);
	pp = ft_calloc(len +1, sizeof(s));
	if (!pp)
		return (NULL);
	aux = 0;
	while (j < len)
	{
		pp[j] = word_dup_after(s, c, aux);
		if (!pp[j])
			return (ft_free_matrix(pp));
		aux = dup_after_pos(s, c, aux);
		j++;
	}
	return (pp);
}

static int	count_valid_args(char **args)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (args[0] && args[0][0] != '<' && args[0][0] != '>')
		count++;
	i = 1;
	while (args[i])
	{
		if (args[i][0] != '<' && args[i][0] != '>' &&
			args[i - 1][0] != '<' && args[i - 1][0] != '>')
		{
			count++;
		}
		i++;
	}
	return (count);
}

// Copiar argumentos válidos a la nueva matriz
static char	**copy_valid_args(char **args, int count)
{
	char	**res;
	int		i;
	int		j;

	res = ft_calloc(count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	j = 0;
	i = 0;
	if (args[0] && args[0][0] != '<' && args[0][0] != '>')
		res[j++] = ft_strdup(args[0]);
	i = 1;
	while (args[i])
	{
		if (args[i][0] != '<' && args[i][0] != '>' &&
			args[i - 1][0] != '<' && args[i - 1][0] != '>')
		{
			res[j++] = ft_strdup(args[i]);
		}
		i++;
	}
	return (res);
}

// Función principal refactorizada
char	**parse_args(char *s)
{
	char	**aux;
	char	**res;
	int		count;

	aux = split_minishell(s, ' ');
	if (!aux)
		return (NULL);
	count = count_valid_args(aux);
	res = copy_valid_args(aux, count);
	ft_free_matrix(aux);
	return (res);
}

/// @brief free an tab
/// @param arr
void	ft_free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
