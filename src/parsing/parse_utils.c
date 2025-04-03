/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:39:33 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 11:20:35 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words(const char *str, char c)
{
	int	i;
	int	trigger;
	int	j;

	i = 0;
	j = 0;
	trigger = 0;
	while (str[j])
	{
		if (str[j] == '"')
		{
			if (str[j - 1] != ' ')
			{
				i++;
			}
			i++;
			j++;
			while (str[j] != '"')
			{
				j++;
			}
		}
		if (str[j] != c && trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (str[j] == c)
			trigger = 0;
		j++;
	}
	return (i);
}

char	*word_dup(char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((finish - start + 1) * sizeof(char));
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	*word_dup_after(char *s, char c, int i)
{
	int		start;
	int		finish;
	char	*res;

	start = 0;
	finish = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			start = i;
			while (s[i] == ' ')
				i++;
			while (s[i] != ' ' && s[i])
				i++;
			finish = i;
			break ;
		}
		i++;
	}
	res = word_dup(s, start, finish);
	return (res);
}

int	dup_after_pos(char *s, char c, int num)
{
	int	i;

	i = num;
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			break ;
		}
		i++;
	}
	return (i);
}

char	*word_dup_before(char *s, char c)
{
	int		i;
	int		start;
	int		finish;
	char	*res;

	i = 0;
	start = 0;
	finish = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			i--;
			finish = i;
			while (s[i] == ' ')
				i--;
			while (s[i] != ' ' && s[i])
				i--;
			start = i;
			break ;
		}
		i++;
	}
	res = word_dup(s, start, finish);
	return (res);
}

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

char	**parse_args(char *s)
{
	int		i;
	int		j;
	int		c;
	char	**aux;
	char	**res;

	i = 0;
	j = 0;
	c = 0;
	aux = split_minishell(s, ' ');
	if (aux[0][0] != '<' && aux[0][0] != '>')
	{
		i++;
		c++;
	}
	while (aux[i])
	{
		if (aux[i][0] != '<' && aux[i][0] != '>'
			&& aux[i - 1][0] != '<' && aux[i - 1][0] != '>')
		{
			c++;
		}
		i++;
	}
	i = 0;
	res = ft_calloc(c +1, sizeof(s));
	if (aux[0][0] != '<' && aux[0][0] != '>')
	{
		res[0] = aux[0];
		i++;
		j++;
	}
	while (aux[i])
	{
		if (aux[i][0] != '<' && aux[i][0] != '>'
			&& aux[i - 1][0] != '<' && aux[i - 1][0] != '>')
		{
			res[j] = ft_strdup(aux[i]);
			j++;
		}
		i++;
	}
	i = 0;
	while (res[i])
	{
		delete_quotes(res[i], '\"');
		delete_quotes(res[i], '\'');
		i++;
	}
	return (res);
}

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

char	**arrdup(char **arr)
{
	char	**aux;
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	aux = ft_calloc(sizeof(char*), i + 1);
	if (!aux)
		return (NULL);
	i = 0;
	while (arr[i] != NULL)
	{
		aux[i] = ft_strdup(arr[i]);
		if (aux[i] == NULL)
		{
			ft_free_arr(aux);
			return (aux);
		}
		i++;
	}
	return (aux);
}

size_t	pos_after_char(char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i + 1);
		i++;
	}
	return (0);
}

int	find_match_quote(char *line, int i, int *num_del, int del)
{
	int	j;

	j = i + 1;
	*num_del += 1;
	while (line[j] && line[j] != del)
		j++;
	if (line[j] == del)
		*num_del += 1;
	else
		return (0);
	return (j - i);
}

int	count_quotes(char *line)
{
	int	i;
	int	s;
	int	d;

	s = 0;
	d = 0;
	i = -1;
	while (line[++i])
	{
		if (line[i] == 34)
			i += find_match_quote(line, i, &d, 34);
		if (line[i] == 39)
			i += find_match_quote(line, i, &s, 39);
	}
	if ((d > 0 && d % 2 != 0) || (s > 0 && s % 2 != 0))
		return (0);
	return (1);
}
