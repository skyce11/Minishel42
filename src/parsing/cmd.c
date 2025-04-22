/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 09:33:25 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/22 11:10:10 by ampocchi         ###   ########.fr       */
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

char	*first_command(char *str)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(sizeof(char) * (ft_strlen_first_word(str) + 1));
	if (!str)
	{
		printf("No hay comando");
		return (0);
	}
	while (str[i] == ' ')
	{
		i++;
	}
	while (str[i] != ' ')
	{
		res[j] = str[i];
		i++;
		j++;
	}
	res[j] = '\0';
	printf("%s\n", res);
	printf("%d\n", i);
	return (res);
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
