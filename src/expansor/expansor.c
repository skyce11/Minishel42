/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:21:27 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/22 11:15:03 by ampocchi         ###   ########.fr       */
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

/// @brief transforma un commando del utilisador que contiene variables
/// en una version interpretada antes que sea executada.
/// @param tools
/// @return la cadena modificada, que contiene el comando con las variables
/// expandidas.
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
	char	*aux2;
	char	*aux3;

	i = 0;
	aux = ft_strdup("\0");
	while (tools->arg_str[i])
	{
		i += digit_after_dollar(i, tools->arg_str);
		// Si se detecta `$?`, se ignora el tratamiento y se devuelve 0.
		if (tools->arg_str[i] == '$' && tools->arg_str[i + 1] == '?')
			return (0);
		// Si `$` está seguido de un carácter válido (ni espacio, ni comillas
		// finales), llama a una función para procesar la expansión de `$`.
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

/// @brief La función loop_dollar procesa una ocurrencia de $VAR en una cadena
// de comandos. Compara la variable con la lista de entorno (envp) y la
/// sustituye por su valor si existe.
/// @param tools
/// @param aux Puntero a una cadena donde se construye el resultado.
/// @param j Posición en la cadena donde comienza $.
/// @return Longitud de la variable procesada (o su valor).
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
