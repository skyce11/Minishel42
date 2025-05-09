/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:49:12 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/09 19:53:55 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	break_heredoc(char *line, int err, t_tools *tools)
{
	if (err == 1)
	{
		ft_putstr_fd("bash: warning: here-document delim", STDOUT_FILENO);
		ft_putendl_fd("ited by end-of-file (wanted `END')", STDOUT_FILENO);
		tools->exit_status = 130;
	}
	else if (err == 2)
		tools->exit_status = 130;
	free(line);
}

static void	heredoc_loop(t_tools *tools, const char *delimiter, int pipe_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (break_heredoc(line, 1, tools));
		if (g_signal == S_CANCEL_EXEC)
			return (break_heredoc(line, 2, tools));
		if (strcmp(line, delimiter) == 0)
			return (break_heredoc(line, 3, tools));
		free(tools->arg_str);
		tools->arg_str = ft_strdup(line);
		tools->arg_str = expansor(tools);
		ft_putendl_fd(tools->arg_str, pipe_fd);
		free(line);
	}
}

int	preprocess_heredoc(t_tools *tools, const char *delimiter)
{
	int		pipe_fd[2];

	signal(SIGINT, SIG_DFL);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	heredoc_loop(tools, delimiter, pipe_fd[1]);
	signal(SIGINT, sigint_handler);
	g_signal = S_BASE;
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
