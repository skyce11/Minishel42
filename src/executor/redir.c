/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:04:01 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/14 13:13:45 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input_redirection(t_redir *redir)
{
	if (dup2(redir->fd, STDIN_FILENO) == -1)
	{
		perror("dup2 failed for STDIN");
		close(redir->fd);
		return ;
	}
}

static void	handle_output_redirection(t_redir *redir, int no_command)
{
	int	fd;

	fd = 0;
	if (no_command)
	{
		if (redir->type == 1)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (redir->type == 3)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("Error opening file");
			return ;
		}
		close(fd);
	}
	else
	{
		if (dup2(redir->fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for STDOUT");
			close(redir->fd);
			return ;
		}
	}
}

/// @brief Prepara las redirecciones para un comando abriendo los archivos
/// necesarios y estableciendo los descriptores de fichero apropiados.
/// @param comando
/// @return 0 si las redirecciones se han configurado correctamente, sino -1
static void	fd_redir(t_tools *tools, t_redir *redir, int no_command)
{
	(void)tools;
	if (redir->type == 0 || redir->type == 2)
		handle_input_redirection(redir);
	else
		handle_output_redirection(redir, no_command);
	if (close(redir->fd) == -1)
		perror("close failed");
}

/// Configura las redirecciones para un comando abriendo los archivos
/// necesarios y ajustando los descriptores de archivo.
/// @param tools Estructura global de herramientas.
/// @param command El comando para el cual se configura la redirección.
/// @return 0 si success, -1 en caso de error.
int	redir_setup(t_tools *tools, t_command *command)
{
	t_redir	*curr_redir;
	t_redir	*first_redir;
	int		no_command;

	curr_redir = command->redir;
	first_redir = command->redir;
	no_command = (command->args == NULL || command->args[0] == NULL);
	while (curr_redir)
	{
		if (file_open(tools, curr_redir) < 1)
			return (-1);
		curr_redir = curr_redir->next;
	}
	curr_redir = first_redir;
	while (curr_redir)
	{
		fd_redir(tools, curr_redir, no_command);
		curr_redir = curr_redir->next;
	}
	return (0);
}
