/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:21:27 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/13 13:20:32 by migonzal         ###   ########.fr       */
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

/// @brief get el codigo de salida pedido con echo $?
/// @param str
/// @param exit_status
/// @return exit code
char	*expand_exit_status(char *str, int exit_status)
{
	char	*result;
	char	*exit_status_str;
	char	*pos;
	char	*substr;
	char	*tmp;

	pos = ft_strstr(str, "$?");
	if (!pos)
		return (str);
	exit_status_str = ft_itoa(exit_status);
	if (!exit_status_str)
		return (str);
	substr = ft_substr(str, 0, pos - str);
	if (!substr)
		return (free(exit_status_str), str);
	tmp = ft_strjoin(substr, exit_status_str);
	free(substr);
	if (!tmp)
		return (free(exit_status_str), str);
	result = ft_strjoin(tmp, pos + 2);
	free(tmp);
	free(exit_status_str);
	free(str);
	return (result);
}

/// @brief transforma un commando del utilisador que contiene variables
/// en una version interpretada antes que sea executada.
/// @param tools
/// @return la cadena modificada, que contiene el comando con las variables
/// expandidas.
// char	*expansor(t_tools *tools)
// {
// 	char	*aux;
// 	int		idx;

// 	aux = NULL;
// 	tools->arg_str = expand_exit_status(tools->arg_str, tools->exit_status);
// 	idx = dollar_after(tools->arg_str);
// 	if (idx >= 0
// 		&& ((idx == 0) || tools->arg_str[idx - 2] != '\'')
// 		&& tools->arg_str[idx] != '\0')
// 	{
// 		aux = detect_dollar(tools);
// 		free(tools->arg_str);
// 		tools->arg_str = aux;
// 	}
// 	return (tools->arg_str);
// }

// static char	*process_dollar(t_tools *tools, char *aux, int *i)
// {
// 	char	*replace_str;
// 	char	*tmp;

// 	replace_str = NULL;
// 	tmp = NULL;
// 	if (tools->arg_str[*i] == '$' && tools->arg_str[*i + 1] == '?')
// 	{
// 		replace_str = ft_itoa(tools->exit_status);
// 		*i += 2;
// 	}
// 	else if (tools->arg_str[*i] == '$' && (tools->arg_str[*i + 1] != ' '
// 			&& (tools->arg_str[*i + 1] != '"'
// 				|| tools->arg_str[*i + 2] != '\0'))
// 		&& tools->arg_str[*i + 1] != '\0')
// 	{
// 		*i += loop_dollar(tools, &aux, *i);
// 		return (aux);
// 	}
// 	else
// 		replace_str = char_to_str(tools->arg_str[(*i)++]);
// 	tmp = ft_strjoin(aux, replace_str);
// 	free(replace_str);
// 	free(aux);
// 	aux = ft_strdup(tmp);
// 	return (free(tmp), aux);
// }

// /// @brief Transforma un comando del usuario que contiene variables en una
// /// versión interpretada antes de ser ejecutada.
// /// @param tools Estructura con las herramientas y datos necesarios para
// /// la gestión del comando.
// /// @return La cadena modificada, que contiene el comando con las variables
// /// expandidas.
// char	*detect_dollar(t_tools *tools)
// {
// 	int		i;
// 	char	*aux;

// 	i = 0;
// 	aux = ft_strdup("\0");
// 	while (tools->arg_str[i])
// 	{
// 		i += digit_after_dollar(i, tools->arg_str);
// 		aux = process_dollar(tools, aux, &i);
// 		if (!aux)
// 			return (NULL);
// 	}
// 	return (aux);
// }


/// @brief transforma un commando del utilisador que contiene variables
/// en una version interpretada antes que sea executada.
/// @param tools
/// @return la cadena modificada, que contiene el comando con las variables
/// expandidas.
char	*expansor(t_tools *tools)
{
	char	*aux;
//	int		idx;
	int		in_squote;
	int		in_dquote;

	aux = NULL;
	in_squote = 0;
	in_dquote = 0;

	// Recorremos la cadena para manejar el estado de las comillas
	for (int i = 0; tools->arg_str[i]; i++)
	{
		if (tools->arg_str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (tools->arg_str[i] == '"' && !in_squote)
			in_dquote = !in_dquote;

		// Si encontramos un dólar que no está dentro de comillas simples
		if (tools->arg_str[i] == '$' && !in_squote)
		{
			aux = detect_dollar(tools);
			free(tools->arg_str);
			tools->arg_str = aux;
			break;
		}
	}
	return (tools->arg_str);
}

static char	*process_dollar(t_tools *tools, char *aux, int *i)
{
	char	*replace_str;
	char	*tmp;

	replace_str = NULL;
	tmp = NULL;
	if (tools->arg_str[*i] == '$' && tools->arg_str[*i + 1] == '?')
	{
		replace_str = ft_itoa(tools->exit_status);
		*i += 2;
	}
	else if (tools->arg_str[*i] == '$' && (tools->arg_str[*i + 1] != ' '
			&& (tools->arg_str[*i + 1] != '"'
				|| tools->arg_str[*i + 2] != '\0'))
		&& tools->arg_str[*i + 1] != '\0')
	{
		*i += loop_dollar(tools, &aux, *i);
		return (aux);
	}
	else
		replace_str = char_to_str(tools->arg_str[(*i)++]);
	tmp = ft_strjoin(aux, replace_str);
	free(replace_str);
	free(aux);
	aux = ft_strdup(tmp);
	return (free(tmp), aux);
}

/// @brief Transforma un comando del usuario que contiene variables en una
/// versión interpretada antes de ser ejecutada.
/// @param tools Estructura con las herramientas y datos necesarios para
/// la gestión del comando.
/// @return La cadena modificada, que contiene el comando con las variables
/// expandidas.
char	*detect_dollar(t_tools *tools)
{
	int		i;
	char	*aux;

	i = 0;
	aux = ft_strdup("\0");
	while (tools->arg_str[i])
	{
		i += digit_after_dollar(i, tools->arg_str);
		aux = process_dollar(tools, aux, &i);
		if (!aux)
			return (NULL);
	}
	return (aux);
}