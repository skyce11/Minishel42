/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:04:01 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/05 06:40:10 by ampocchi         ###   ########.fr       */
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

/// @brief Prepara las redirecciones para un comando abriendo los archivos
/// necesarios y estableciendo los descriptores de fichero apropiados.
/// @param comando
/// @return 0 si las redirecciones se han configurado correctamente, sino -1
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
