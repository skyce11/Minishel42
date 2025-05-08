/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_child_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:08:09 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/08 18:08:31 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_child_status(int status, int fd, char *result, t_tools *tools)
{
	int	sig;

	sig = -1;
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		close(fd);
		free(result);
		if (sig == SIGINT)
			tools->exit_status = 130;
		else
			tools->exit_status = 2;
		return (EXIT_SUCCESS);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
	{
		close(fd);
		free(result);
		tools->exit_status = WEXITSTATUS(status);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
