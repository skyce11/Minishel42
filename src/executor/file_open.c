/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:02:06 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/09 19:52:04 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_file_open_output(t_tools *tools, t_redir *redir)
{
	int	mode;

	(void)tools;
	mode = O_CREAT | O_RDWR;
	if (redir->type == 3)
		mode = mode | O_APPEND;
	redir->fd = open(redir->file, mode, S_IRGRP | S_IWUSR | S_IRUSR | S_IROTH);
	return (redir->fd);
}

static int	get_open_mode(t_tools *tools, t_redir *redir)
{
	(void)tools;
	if (redir->type == 0 || redir->type == 2)
		return (R_OK);
	else
		return (R_OK | W_OK);
}

static int	open_existing_file(t_tools *tools, t_redir *redir)
{
	int	mode;

	mode = get_open_mode(tools, redir);
	if (access(redir->file, mode) != -1)
	{
		if (redir->type == 3)
			mode = O_RDWR | O_APPEND;
		redir->fd = open(redir->file, mode);
		if (redir->fd == -1)
			return (-1);
		return (redir->fd);
	}
	else
	{
		ft_putstr_fd("Minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->file, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		return (-1);
	}
}

int	file_open(t_tools *tools, t_redir *redir)
{
	if (redir->type == 2)
	{
		redir->fd = preprocess_heredoc(tools, redir->file);
		if (redir->fd == -1)
		{
			ft_putstr_fd("Error al procesar heredoc\n", STDERR_FILENO);
			return (-1);
		}
		return (redir->fd);
	}
	if (access(redir->file, F_OK) != -1)
		return (open_existing_file(tools, redir));
	if (redir->type == 1 || redir->type == 3)
		return (handle_file_open_output(tools, redir));
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	ft_putstr_fd(redir->file, STDERR_FILENO);
	ft_putstr_fd(": File or directory does not exist\n", STDERR_FILENO);
	tools->exit_status = 127;
	return (0);
}
