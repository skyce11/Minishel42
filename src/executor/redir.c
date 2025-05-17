/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:04:01 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/17 16:51:08 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input_redirection(t_redir *redir)
{
	if (redir->fd == -1)
	{
		perror("Error opening file for input redirection");
		return ;
	}
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
	if (redir->fd != -1 && !no_command)
	{
		if (close(redir->fd) == -1)
			perror("close failed");
	}
}

static int	redir_handle(t_tools *tools, t_redir *curr_redir, int no_command)
{
	t_redir	*temp;
	int		is_input;

	temp = curr_redir;
	while (temp)
	{
		is_input = (temp->type == 0 || temp->type == 2);
		if (file_open(tools, temp) < 1)
		{
			if (is_input)
				return (-1);
			if (no_command)
				return (-1);
			temp->fd = -1;
		}
		temp = temp->next;
	}
	return (0);
}

int	redir_setup(t_tools *tools, t_command *command)
{
	t_redir	*first_redir;
	t_redir	*curr_redir;
	int		no_command;
	int		res;

	first_redir = command->redir;
	curr_redir = command->redir;
	no_command = 0;
	res = 0;
	if (command->args == NULL || command->args[0] == NULL)
		no_command = 1;
	res = redir_handle(tools, curr_redir, no_command);
	if (res == -1)
		return (-1);
	curr_redir = first_redir;
	while (curr_redir)
	{
		if (curr_redir->fd != -1)
			fd_redir(tools, curr_redir, no_command);
		curr_redir = curr_redir->next;
	}
	return (0);
}
