/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 12:13:15 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 11:04:41 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_free_matrix(char **mtx)
{
	int	i;

	if (!mtx)
		return (NULL);
	i = 0;
	while (mtx[i])
	{
		free(mtx[i]);
		i++;
	}
	free(mtx);
	return (NULL);
}

char	**split_minishell(char *s, char z)
{
	char	**pp;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = count_tokens(s, z);
	if (len == 0)
		return (NULL);
	pp = ft_calloc(len +1, sizeof(s));
	if (!pp)
		return (NULL);
	i = 0;
	while (*s && i < len)
	{
		while (*s && (*s == z || *s == '\t'))
			s++;
		pp[i] = ft_substr(s, 0, count_string(s, z));
		if (!pp[i])
			return (ft_free_matrix(pp));
		if (*s)
			s = cross_string(s, NULL, z);
		i++;
	}
	return (pp);
}

size_t	count_tokens(char *s, char z)
{
	size_t	i;

	i = 0;
	while (s && *s)
	{
		while (s && *s && (*s == z || *s == '\t'))
			s++;
		if (*s)
			i++;
		s = cross_string(s, NULL, z);
	}
	return (i);
}

char	*cross_string(char *s, size_t *i, char z)
{
	char	c;
	size_t	len;
	int		count;

	c = 0;
	len = 0;
	count = 0;
	if (*s == '\'' || *s == '\t')
	{
		c = *s;
		count = 1;
		len++;
	}
	if (*s && (*s == c || *s == z || *s == '\t'))
		s++;
	s = get_end_str(s, c, &len, count, z);
	if (i)
		*i = len;
	return (s);
}

char	*get_end_str(char *s, char c, size_t *len, int count, char z)
{
	while (*s && ((*s != z && *s != '\t') || ((*s == z && count == 1)
				|| (*s == '\t' && count == 1))))
	{
		if (c != 0 && (*s == c))
		{
			count = !count;
			c = 0;
		}
		else if (c == 0 && (*s == '\'' || *s == '\"'))
		{
			c = *s;
			count = !count;
		}
		s++;
		*len = *len +1;
	}
	return (s);
}

size_t	count_string(char *s, char z)
{
	size_t	i;

	i = 0;
	cross_string(s, &i, z);
	return (i);
}
