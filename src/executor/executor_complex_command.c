/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_complex_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:30:00 by sperez-s          #+#    #+#             */
/*   Updated: 2025/04/18 14:40:33 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_file_descriptors(t_command *curr_command, t_pipes_command pipes,
	unsigned int i)
{
	if (curr_command->next)
	{
		dup2(pipes.curr->pipe[1], STDOUT_FILENO);
		close(pipes.curr->pipe[0]);
		close(pipes.curr->pipe[1]);
	}
	if (i != 0)
	{
		dup2(pipes.prev->pipe[0], STDIN_FILENO);
		close(pipes.prev->pipe[0]);
	}
	if (is_builtin(curr_command)
		&& ft_strcmp(curr_command->args[0], "exit") == 0)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
}

static int	piped_command_child(t_command *curr_command,
		t_pipes_command pipes, t_tools *tools, unsigned int i)
{
	signal(SIGINT, SIG_DFL);
	set_file_descriptors(curr_command, pipes, i);
	run_command(curr_command, tools);
	if (is_builtin(curr_command))
	{
		close(STDOUT_FILENO);
		exit(0);
	}
	return (-1);
}

static int	exec_piped_command(t_pipe *ps, t_tools *tools,
	t_command *curr_command, unsigned int i)
{
	int				pid;
	int				child_status;
	t_pipes_command	pipes;

	pipes.prev = obtain_related_pipe_from_list(ps, i, 1);
	pipes.curr = obtain_related_pipe_from_list(ps, i, 0);
	if (fill_command_from_env(curr_command, tools) != -1)
	{
		if (curr_command->next != NULL)
			pipe(pipes.curr->pipe);
		pid = fork();
		if (pid == 0)
			return (piped_command_child(curr_command, pipes, tools, i));
		else
		{
			if (curr_command->next != NULL)
				close(pipes.curr->pipe[1]);
			if (pipes.prev)
				close(pipes.prev->pipe[0]);
			waitpid(pid, &child_status, 0);
			handle_status(child_status, tools);
		}
	}
	return (0);
}

/// @brief Maneja la ejecución de comandos compuestos, como los que utilizan
/// tuberías. Crea una lista de tuberías, ejecuta cada comando con las
/// redirecciones apropiadas, y limpia los recursos utilizados.
/// @param tools
/// @param size Número de comandos en la cadena compuesta.
/// @return 0 en caso de éxito, -1 en caso de error al crear tuberías,
/// o 1 si un comando falla.
int	exec_compound_command(t_tools *tools, unsigned int size)
{
	t_pipe			*ps;
	unsigned int	i;
	t_command		*curr_command;

	i = 0;
	ps = create_pipe_list(size);
	if (!ps)
	{
		write(2, "ERROR: Could't create pipe list\n", 33);
		return (-1);
	}
	curr_command = tools->command;
	while (i < size)
	{
		if (exec_piped_command(ps, tools, curr_command, i) != 0)
			return (1);
		if (curr_command)
			curr_command = curr_command->next;
		i++;
	}
	cleanse_pipe_list(&ps);
	return (0);
}
