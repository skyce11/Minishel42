/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 13:25:07 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/17 17:05:10 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Run the command depends if is a builton or not.
/// @param command The command to be executed.
/// @param tools
/// @return Void
void	run_command(t_command *command, t_tools *tools)
{
	if (redir_setup(tools, command) == 0)
	{
		if (command->args == NULL || command->args[0] == NULL)
			return ;
		if (is_builtin(command))
			ft_builtin(command, tools);
		else
		{
			execve(command->args[0], command->args, tools->envp);
		}
	}
}

static int	save_stdin(void)
{
	int	orig;

	orig = dup(STDIN_FILENO);
	if (orig == -1)
		perror("dup failed");
	return (orig);
}

static void	restore_stdin(int orig_stdin)
{
	if (dup2(orig_stdin, STDIN_FILENO) == -1)
		perror("dup2 failed on restore");
	close(orig_stdin);
}

/// @brief Esta función `executor` se encarga de gestionar la ejecución de los
/// comandos introducidos por el usuario. Determina si un único comando o un
/// comando (con tuberías, etc.) debe ser ejecutado.
/// @param tools Estructura que contiene los datos del shell,
/// incluyendo la lista de comandos.
/// @return 0 en caso de error, o el resultado de la ejecución del comando.

void	executor(t_tools *tools)
{
	unsigned int	size;
	int				orig_stdin;

	size = get_command_list_size(tools->command);
	orig_stdin = save_stdin();
	tools->exit_status = 0;
	if (size < 1)
		ft_putendl_fd("ERROR: empty command list", 1);
	else if (size == 1)
	{
		if (tools->command->args == NULL || tools->command->args[0] == NULL)
		{
			redir_setup(tools, tools->command);
			restore_stdin(orig_stdin);
			return ;
		}
		exec_single_command(tools->command, tools);
	}
	else
		exec_compound_command(tools, size);
	restore_stdin(orig_stdin);
}
