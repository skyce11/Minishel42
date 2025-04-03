/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_single_command.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sperez-s <sperez-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:34:38 by sperez-s          #+#    #+#             */
/*   Updated: 2025/03/26 21:08:35 by sperez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_builtin_in_parent_process(t_command *command, t_tools *tools)
{
	int	stdin_fd;
	int	stdout_fd;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	run_command(command, tools);
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}

static int	fork_single_command(t_command *command, t_tools *tools)
{
	int	pid;
	int	status;

	if (fill_command_from_env(command, tools) == -1)
		return (-1);
	g_signal = S_CMD;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, sigquit_handler);
		run_command(command, tools);
		return (-1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		handle_status(status, tools);
	}
	else
		return (-1);
	g_signal = S_BASE;
	signal(SIGQUIT, SIG_IGN);
	return (status);
}

int	exec_single_command(t_command *command, t_tools *tools)
{
	int	status;

	if (is_builtin(command))
	{
		run_builtin_in_parent_process(command, tools);
		return (0);
	}
	status = fork_single_command(command, tools);
	return (status);
}
