/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expensor_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:10:12 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/09 20:10:13 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_env_value(t_tools *tools, char **aux, int j)
{
	int		i;
//	int		res;
	char	*aux2;
	char	*aux3;

	i = 0;
//	res = 0;
	while (tools->envp[i])
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
			return (equal_after(tools->envp[i]));
		}
		i++;
	}
	return (0);
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
	int		res;
	char	*aux2;
	char	*aux3;
	int		var_length;

	res = find_env_value(tools, aux, j);
	if (res == 0)
	{
		var_length = after_dollar_lenght(tools->arg_str, j) - j;
		aux2 = ft_strndup(tools->arg_str + j, var_length);
		aux3 = ft_strjoin(*aux, aux2);
		free(*aux);
		*aux = aux3;
		free(aux2);
		res = var_length;
	}
	return (res);
}
