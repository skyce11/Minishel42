/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sperez-s <sperez-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:04:01 by sperez-s          #+#    #+#             */
/*   Updated: 2025/03/26 21:09:59 by sperez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fd_redir(t_redir *redir)
{
	if (redir->type == 0 || redir->type == 2)
		dup2(redir->fd, STDIN_FILENO);
	else
		dup2(redir->fd, STDOUT_FILENO);
	close(redir->fd);
}

int	redir_setup(t_command *command)
{
	t_redir	*curr_redir;
	t_redir	*first_redir;

	curr_redir = command->redir;
	first_redir = command->redir;
	while (curr_redir)
	{
		if (file_open(curr_redir) < 1)
			return (-1);
		curr_redir = curr_redir->next;
	}
	curr_redir = first_redir;
	while (curr_redir)
	{
		fd_redir(curr_redir);
		curr_redir = curr_redir->next;
	}
	return (0);
}
