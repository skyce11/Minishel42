/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:55:19 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 16:42:37 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words(const char *str, char c)
{
	int	count;
	int	trigger;
	int	i;

	count = 0;
	trigger = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (i > 0 && str[i - 1] != ' ')
				count++;
			while (str[++i] && str[i] != '"')
				;
		}
		if (str[i] != c && !trigger)
		{
			trigger = 1;
			count++;
		}
		else if (str[i++] == c)
			trigger = 0;
	}
	return (count);
}

char	*word_dup(char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((finish - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
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
