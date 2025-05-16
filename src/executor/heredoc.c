/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:49:12 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/16 14:42:03 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	break_heredoc(char *line, int err, t_tools *tools, const char *del)
{
	if (err == 1)
	{
		ft_putstr_fd("bash: warning: here-document delim", STDOUT_FILENO);
		ft_putstr_fd("ited by end-of-file (wanted `", STDOUT_FILENO);
		ft_putstr_fd((char *)del, STDOUT_FILENO);
		ft_putendl_fd("')", STDOUT_FILENO);
		tools->exit_status = 0;
	}
	else if (err == 2 || err == 4)
	{
		tools->exit_status = 130;
	}
	free(line);
}

static void	heredoc_loop(t_tools *tools, const char *delimiter, int pipe_fd)
{
	char	*line;

	g_signal = 0;
	signal(SIGINT, sigint_handler_heredoc);
	while (1)
	{
		line = readline("> ");
		if (g_signal == 2)
			return (break_heredoc(line, 2, tools, delimiter));
		if (line == NULL)
			return (break_heredoc(line, 1, tools, delimiter));
		if (strcmp(line, delimiter) == 0)
			return (break_heredoc(line, 3, tools, delimiter));
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
	int		status;
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		heredoc_loop(tools, delimiter, pipe_fd[1]);
		close(pipe_fd[1]);
		exit(tools->exit_status);
	}
	else
	{
		close(pipe_fd[1]);
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, sigint_handler);
		handle_status(status, tools);
		return (pipe_fd[0]);
	}
}
