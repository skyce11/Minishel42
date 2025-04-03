/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:21:27 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 10:26:54 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;

	if (needle[0] == '\0')
		return ((char *)haystack);
	i = 0;
	while (haystack[i] != '\0')
	{
		j = 0;
		while (needle[j] != '\0' && haystack[i + j] == needle[j])
		{
			if (needle[j + 1] == '\0')
				return ((char *)&haystack[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}

char	*expand_exit_status(char *str, int exit_status)
{
	char	*result;
	char	*exit_status_str;
	char	*pos;

	pos = ft_strstr(str, "$?");
	if (!pos)
		return (str);
	exit_status_str = ft_itoa(exit_status);
	if (!exit_status_str)
		return (str);
	result = ft_strjoin(ft_substr(str, 0, pos - str), exit_status_str);
	result = ft_strjoin(result, pos + 2);
	free(exit_status_str);
	free(str);
	return (result);
}

char	*expansor(t_tools *tools)
{
	char	*aux;

	aux = NULL;
	tools->arg_str = expand_exit_status(tools->arg_str, tools->exit_status);
	if (tools->arg_str[dollar_after(tools->arg_str) - 2]
		!= '\'' && dollar_after(tools->arg_str) != 0
		&& tools->arg_str[dollar_after(tools->arg_str)] != '\0')
	{
		aux = detect_dollar(tools);
		free(tools->arg_str);
		tools->arg_str = aux;
	}
	return (tools->arg_str);
}

char	*detect_dollar(t_tools *tools)
{
	int		i;
	char	*aux;
	char	*aux2;
	char	*aux3;

	i = 0;
	aux = ft_strdup("\0");
	while (tools->arg_str[i])
	{
		i += digit_after_dollar(i, tools->arg_str);
		if (tools->arg_str[i] == '$' && tools->arg_str[i + 1] == '?')
			return (0);
		else if (tools->arg_str[i] == '$' && (tools->arg_str[i + 1] != ' '
				&& (tools->arg_str[i + 1] != '"' || tools->arg_str[i + 2]
					!= '\0')) && tools->arg_str[i + 1] != '\0')
			i += loop_dollar(tools, &aux, i);
		else
		{
			aux2 = char_to_str(tools->arg_str[i++]);
			aux3 = ft_strjoin(aux, aux2);
			free(aux);
			aux = aux3;
			free(aux2);
		}
	}
	return (aux);
}

int	loop_dollar(t_tools *tools, char **aux, int j)
{
	int		i;
	int		res;
	char	*aux2;
	char	*aux3;
	int		var_length;

	i = 0;
	res = 0;
	var_length = 0;
	while (tools-> envp[i])
	{
		if (ft_strncmp(tools->arg_str + j + 1, tools->envp[i],
				equal_after(tools->envp[i]) - 1) == 0
			&& after_dollar_lenght(tools->arg_str, j) - j
			== (int) equal_after(tools->envp[i]))
		{
			aux2 = ft_strdup(tools->envp[i] + equal_after(tools->envp[i]));
			aux3 = ft_strjoin(*aux, aux2);
			free(*aux);
			*aux = aux3;
			free(aux2);
			res = equal_after(tools->envp[i]);
		}
		i++;
	}
	if (res == 0)
	{
		var_length = after_dollar_lenght(tools->arg_str, j) - j;
		aux2 = strndup(tools->arg_str + j, var_length);
		aux3 = ft_strjoin(*aux, aux2);
		free(*aux);
		*aux = aux3;
		free(aux2);
		res = var_length;
	}
	return (res);
}
