/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:49:12 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/17 21:03:51 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	break_heredoc(char *line, int err, t_tools *tools, const char *del)
{
	if (err == 1)
	{
		ft_putstr_fd("minishell: warning: here-document delim", STDOUT_FILENO);
		ft_putstr_fd("ited by end-of-file (wanted `", STDOUT_FILENO);
		ft_putstr_fd((char *)del, STDOUT_FILENO);
		ft_putendl_fd("')", STDOUT_FILENO);
		tools->exit_status = 0;
	}
	else if (err == 2)
	{
		tools->exit_status = 130;
	}
	free(line);
	exit(tools->exit_status);
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
			break_heredoc(line, 2, tools, delimiter);
		if (line == NULL)
			break_heredoc(line, 1, tools, delimiter);
		if (strcmp(line, delimiter) == 0)
			return (break_heredoc(line, 3, tools, delimiter));
		free(tools->arg_str);
		tools->arg_str = ft_strdup(line);
		tools->arg_str = expansor(tools);
		ft_putendl_fd(tools->arg_str, pipe_fd);
		free(line);
	}
}

int	parent_process(t_tools *tools, int pipe_fd[2], pid_t pid, int status)
{
	close(pipe_fd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler_heredoc);
	handle_status(status, tools);
	if (tools->exit_status != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (0);
}

int	preprocess_heredoc(t_tools *tools, const char *delimiter)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid;

	status = 0;
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
		if (parent_process(tools, pipe_fd, pid, status) == -1)
			return (-1);
		return (pipe_fd[0]);
	}
}
