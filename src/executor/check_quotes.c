/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:08:13 by ampocchi          #+#    #+#             */
/*   Updated: 2025/05/12 15:09:11 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void update_quotes(const char *line, int *in_dquote, int *in_squote)
{
	int i = 0;

	while (line[i])
	{
		if (*in_dquote)
		{
			if (line[i] == '"')
				*in_dquote = 0;
		}
		else if (*in_squote)
		{
			if (line[i] == '\'')
				*in_squote = 0;
		}
		else
		{
			if (line[i] == '"')
				*in_dquote = 1;
			else if (line[i] == '\'')
				*in_squote = 1;
		}
		i++;
	}
}

void	heredoc_update_buffer(char **buffer, int *dquote, int *squote, int *fd)
{
	char	*next_line;
	char	*temp;
	char	*str_err;

	str_err = "bash: unexpected EOF while looking for matching ";
	next_line = readline("> ");
	if (!next_line)
	{
		ft_putstr_fd(str_err, STDOUT_FILENO);
		if (*dquote)
			ft_putendl_fd("`\"'", STDOUT_FILENO);
		else if (*squote)
			ft_putendl_fd("`\''", STDOUT_FILENO);
		ft_putendl_fd("syntax error: unexpected end of file", STDOUT_FILENO);
		free(*buffer);
		close(fd[1]);
		close(fd[0]);
		exit(F_QUOTE);
	}
	temp = ft_strjoin(*buffer, "\n");
	free(*buffer);
	*buffer = ft_strjoin(temp, next_line);
	free(temp);
	free(next_line);
}

void	child_heredoc(int fd[2], int in_dquote, int in_squote, t_tools *tools)
{
	char	*buffer;

	signal(SIGINT, SIG_DFL);
	close(fd[0]);
	buffer = ft_strdup(tools->arg_str);
	if (!buffer)
		exit(EXIT_FAILURE);
	while (in_dquote || in_squote)
	{
		heredoc_update_buffer(&buffer, &in_dquote, &in_squote, fd);
		in_dquote = 0;
		in_squote = 0;
		update_quotes(buffer, &in_dquote, &in_squote);
	}
	write(fd[1], buffer, ft_strlen(buffer));
	free(buffer);
	close(fd[1]);
	signal(SIGINT, sigint_handler);
	exit(EXIT_SUCCESS);
}

static int	parent_heredoc(int fd[2], t_tools *tools, pid_t pid)
{
	int		status;
	char	*result;
	char	*line;
	char	*temp;

	result = ft_strdup("");
	close(fd[1]);
	waitpid(pid, &status, 0);
	handle_status(status, tools);
	if (check_child_status(status, fd[0], result, tools) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	line = get_next_line(fd[0]);
	while (line != NULL)
	{
		temp = result;
		result = ft_strjoin(result, line);
		free(temp);
		free(line);
		if (!result)
			return (close(fd[0]), EXIT_FAILURE);
		line = get_next_line(fd[0]);
	}
	close(fd[0]);
	free(tools->arg_str);
	tools->arg_str = result;
	g_signal = S_BASE;
	return (EXIT_FAILURE);
}

int	check_quotes(int in_dquote, int in_squote, t_tools *tools)
{
	int		fd[2];
	pid_t	pid;

	update_quotes(tools->arg_str, &in_dquote, &in_squote);
	g_signal = S_QUOTE;
	if (!in_dquote && !in_squote)
	{
		g_signal = S_BASE;
		return (EXIT_FAILURE);
	}
	if (pipe(fd) == -1)
		return (perror("pipe"), EXIT_SUCCESS);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), EXIT_SUCCESS);
	else if (pid == 0)
		child_heredoc(fd, in_dquote, in_squote, tools);
	else
		return (parent_heredoc(fd, tools, pid));
	g_signal = S_BASE;
	return (EXIT_SUCCESS);
}
