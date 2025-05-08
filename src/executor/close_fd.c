#include "minishell.h"

/// @brief Configures file descriptors for a piped command.
/// Also ensures proper resource cleanup.
/// @param curr_command The current command being executed.
/// @param pipes Structure managing inter-process communication.
/// @param i Index of the command in the sequence.
/// @return Void
void	safe_close(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

void	close_safe(t_command *curr_command, t_pipes_command	pipes)
{
	if (curr_command->next != NULL && pipes.curr->pipe[1] >= 0)
		close(pipes.curr->pipe[1]);
	if (pipes.prev && pipes.prev->pipe[0] >= 0)
		close(pipes.prev->pipe[0]);
}
