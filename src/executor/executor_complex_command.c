/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_complex_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:30:00 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/10 15:40:32 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Fonction générique pour appliquer dup2 avec vérification */
void	safe_dup2(int oldfd, int newfd)
{
	if (oldfd != -1)
	{
		if (dup2(oldfd, newfd) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
}

void	set_file_descriptors(t_command *curr_command, t_pipes_command pipes,
						unsigned int i)
{
	if (curr_command->next)
	{
		if (pipes.curr->pipe[1] == -1)
		{
			if (pipe(pipes.curr->pipe) < 0)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		safe_dup2(pipes.curr->pipe[1], STDOUT_FILENO);
		safe_close(&pipes.curr->pipe[0]);
		safe_close(&pipes.curr->pipe[1]);
	}
	if (i != 0 && pipes.prev)
	{
		safe_dup2(pipes.prev->pipe[0], STDIN_FILENO);
		safe_close(&pipes.prev->pipe[0]);
	}
	if (is_builtin(curr_command) && strcmp(curr_command->args[0], "exit") == 0)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
}

/// @brief Executes a command as part of a piped sequence.
/// It sets up signal handling, adjusts file descriptors for input/output,
/// runs the command, and handles built-in command cases.
/// @param curr_command The command to be executed.
/// @param pipes Structure holding pipes used for communication.
/// @param tools
/// @param i Index of the command in the sequence.
/// @return -1 in case of an error; exits with 0 for built-in commands.
static int	piped_command_child(t_command *curr_command,
		t_pipes_command pipes, t_tools *tools, unsigned int i)
{
	set_file_descriptors(curr_command, pipes, i);
	run_command(curr_command, tools);
	if (is_builtin(curr_command))
	{
		close(STDOUT_FILENO);
		exit(0);
	}
	return (-1);
}

/// @brief Executes a piped command within a compound command structure.
/// It sets up the necessary pipes, forks a child process to execute the
/// command, and manages resource cleanup.
/// @param ps List of pipes used for inter-process communication.
/// @param tools
/// @param curr_command The command to be executed.
/// @param i Index of the command within the compound command sequence.
/// @return 0 on success, or the return value of the child process.
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
			if (pipe(pipes.curr->pipe) < 0)
				return (-1);
		pid = fork();
		if (pid == 0)
			return (piped_command_child(curr_command, pipes, tools, i));
		else
		{
			close_safe(curr_command, pipes);
			waitpid(pid, &child_status, 0);
			handle_status(child_status, tools);
			tools->exit_status = 0;
		}
	}
	else
		return (close_safe(curr_command, pipes), tools->exit_status);
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
		ft_putendl_fd("ERROR: Could't create pipe list\n", STDERR_FILENO);
		return (-1);
	}
	curr_command = tools->command;
	while (i++ < size)
	{
		g_signal = S_SIGINT;
		if (exec_piped_command(ps, tools, curr_command, i) != 0)
		{
			cleanse_pipe_list(&ps);
			return (1);
		}
		g_signal = S_BASE;
		curr_command = curr_command->next;
	}
	cleanse_pipe_list(&ps);
	return (tools->exit_status);
}
