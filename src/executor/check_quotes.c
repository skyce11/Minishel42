#include "minishell.h"

static void	update_quotes(const char *line, int *in_dquote, int *in_squote)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"' && !(*in_squote))
			*in_dquote = !(*in_dquote);
		else if (line[i] == '\'' && !(*in_dquote))
			*in_squote = !(*in_squote);
		i++;
	}
}

static void	heredoc_update_buffer(char **buffer, int *in_dquote, int *in_squote)
{
	char	*next_line;
	char	*temp;

	next_line = readline("> ");
	if (!next_line)
	{
		ft_putendl_fd("syntax error: unexpected end of file", STDOUT_FILENO);
		free(*buffer);
		exit(EXIT_FAILURE);
	}
	temp = ft_strjoin(*buffer, "\n");
	free(*buffer);
	*buffer = ft_strjoin(temp, next_line);
	free(temp);
	free(next_line);
	*in_dquote = 0;
	*in_squote = 0;
	update_quotes(*buffer, in_dquote, in_squote);
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
		heredoc_update_buffer(&buffer, &in_dquote, &in_squote);
	write(fd[1], buffer, ft_strlen(buffer));
	free(buffer);
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

static int	check_child_status(int status, int fd, char *result)
{
	if (WIFSIGNALED(status) || (WIFEXITED(status)
			&& WEXITSTATUS(status) != EXIT_SUCCESS))
	{
		close(fd);
		free(result);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
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
	if (check_child_status(status, fd[0], result) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	line = get_next_line(fd[0]);
	while (line != NULL)
	{
		temp = result;
		result = ft_strjoin(result, line);
		free(temp);
		free(line);
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
	g_signal = S_HEREDOC;
	if (!in_dquote && !in_squote)
		return (EXIT_FAILURE);
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (EXIT_SUCCESS);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (EXIT_SUCCESS);
	}
	else if (pid == 0)
		child_heredoc(fd, in_dquote, in_squote, tools);
	else
		return (parent_heredoc(fd, tools, pid));
	return (EXIT_SUCCESS);
}
