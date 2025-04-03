/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sperez-s <sperez-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:02:06 by sperez-s          #+#    #+#             */
/*   Updated: 2025/03/26 21:10:40 by sperez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc(t_redir *redir)
{
	char	*line;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	g_signal = S_HEREDOC;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	g_signal = S_BASE;
	close(pipe_fd[1]);
	redir->fd = pipe_fd[0];
	return (1);
}

static int	handle_file_open_output(t_redir *redir)
{
	int	mode;

	mode = O_CREAT | O_RDWR;
	if (redir->type == 3)
		mode = mode | O_APPEND;
	redir->fd = open(redir->file, mode, S_IRGRP | S_IWUSR | S_IRUSR | S_IROTH);
	return (redir->fd);
}

static int	get_open_mode(t_redir *redir)
{
	if (redir->type == 0 || redir->type == 2)
		return (R_OK);
	else
		return (R_OK | W_OK);
}

static int	open_existing_file(t_redir *redir)
{
	int	mode;

	mode = get_open_mode(redir);
	if (access(redir->file, mode) != -1)
	{
		if (redir->type == 3)
			mode = O_RDWR | O_APPEND;
		redir->fd = open(redir->file, mode);
		return (redir->fd);
	}
	else
	{
		printf("Minishell: %s: Permission denied\n", redir->file);
		return (0);
	}
}

int	file_open(t_redir *redir)
{
	if (access(redir->file, F_OK) != -1)
		return (open_existing_file(redir));
	else if (redir->type == 1 || redir->type == 3)
		return (handle_file_open_output(redir));
	else if (redir->type == 2)
		return (handle_heredoc(redir));
	else
	{
		printf("Minishell: %s: File or directory does not exist\n",
			redir->file);
		return (0);
	}
	return (1);
}
