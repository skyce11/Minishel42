/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sperez-s <sperez-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 13:25:07 by sperez-s          #+#    #+#             */
/*   Updated: 2025/03/26 19:34:27 by sperez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_command(t_command *command, t_tools *tools)
{
	if (redir_setup(command) == 0)
	{
		if (is_builtin(command))
			ft_builtin(command, tools);
		else
			execve(command->args[0], command->args, tools->envp);
	}
}

int	executor(t_tools *tools)
{
	unsigned int	size;

	size = get_command_list_size(tools->command);
	if (size < 1)
	{
		printf("ERROR: empty command list\n");
		return (0);
	}
	if (size == 1)
		return (exec_single_command(tools->command, tools));
	return (exec_compound_command(tools, size));
}
