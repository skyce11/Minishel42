/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 09:33:25 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 15:17:12 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Checks for invalid pipe syntax in a string.
/// Scans the input string to detect sequences like "| |" which are incorrect.
/// If an error is found, it prints a message and returns 0.
/// @param str The string to validate.
/// @return 1 if the syntax is correct, 0 if an error is detected.
int	validate_pipes(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|' && str[i + 1] == ' ' && str[i + 2] == '|')
		{
			printf("Error: Invalid syntax `| |'\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_strlen_first_word(char *str)
{
	int	i;
	int	cont;

	cont = 0;
	i = 0;
	while (str[i] == ' ')
		i++;
	while (str[i] != ' ')
	{
		i++;
		cont++;
	}
	return (cont);
}

char	*arg_quote(char *str)
{
	int		i;
	char	**res;

	i = 0;
	res = ft_split(str, '"');
	while (res[i])
	{
		printf("%s\n", res[i]);
		i++;
	}
	return (0);
}
