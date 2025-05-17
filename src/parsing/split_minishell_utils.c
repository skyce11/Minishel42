/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_minishell_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:54:10 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/17 20:48:55 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Calculates the length of a substring until a delimiter. Uses
/// `cross_string` to find the endpoint of the substring and returns its length.
/// @param s The string to analyze.
/// @param z The delimiter character.
/// @return The length of the substring before the delimiter.
size_t	count_string(char *s, char z)
{
	size_t	i;

	i = 0;
	cross_string(s, &i, z);
	return (i);
}

/// @brief Frees a dynamically allocated matrix (array of strings).
/// @param mtx The matrix (array of strings) to free.
/// @return Always returns NULL after freeing memory.
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

void	ft_strim_without_leaks(t_tools *tools)
{
	char	*temp;

	temp = tools->arg_str;
	tools->arg_str = ft_strtrim(tools->arg_str, " \t");
	free(temp);
	if (tools->arg_str[0] == '\0' || !validate_pipes(tools->arg_str))
	{
		reset_tools(tools);
		return ;
	}
	if (tools->arg_str)
		add_history(tools->arg_str);
}
/// @brief update the nbr of SHLVL.
/// @param tools
/// @param flag if flags = 1 -> increment ; if flags = 0 -> decrement;

void	update_shlvl(t_tools *tools)
{
	int		i;
	int		shlvl;
	char	*new_val;
	char	*entry;

	i = 0;
	while (tools->envp[i++])
	{
		if (!ft_strncmp(tools->envp[i], "SHLVL=", 6))
		{
			shlvl = ft_atoi(tools->envp[i] + 6);
			shlvl++;
			new_val = ft_itoa(shlvl);
			entry = ft_strjoin("SHLVL=", new_val);
			free(new_val);
			free(tools->envp[i]);
			tools->envp[i] = entry;
			return ;
		}
	}
}
