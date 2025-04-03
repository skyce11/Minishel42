/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sperez-s <sperez-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:27:37 by sperez-s          #+#    #+#             */
/*   Updated: 2025/03/26 21:09:07 by sperez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_command_in_route(t_command *command, char *path)
{
	char	*joined;
	int		found;

	found = 0;
	joined = ft_strjoin(path, command->args[0]);
	if (access(joined, R_OK | X_OK) != -1)
	{
		free(command->args[0]);
		command->args[0] = joined;
		found = 1;
	}
	else
		free(joined);
	return (found);
}

int	fill_command_from_env(t_command *command, t_tools *tools)
{
	int	found;
	int	i;

	if (is_builtin(command))
		return (0);
	found = 0;
	i = 0;
	while (tools->paths[i] && !found)
	{
		if (access(command->args[0], R_OK | X_OK) != -1)
			found = 1;
		else
		{
			found = find_command_in_route(command, tools->paths[i]);
			i++;
		}
	}
	if (found == 0)
	{
		printf("%s: command not found\n", command->args[0]);
		return (-1);
	}
	errno = 0;
	return (0);
}

int	get_command_list_size(t_command *list)
{
	int	i;

	i = 0;
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

t_pipe	*obtain_related_pipe_from_list(t_pipe *ps,
		unsigned int pos, int is_prev)
{
	t_pipe			*curr_pipe;
	unsigned int	i;

	curr_pipe = ps;
	i = 1;
	if (!is_prev)
		i = 0;
	while (i < pos && curr_pipe)
	{
		curr_pipe = curr_pipe->next;
		i++;
	}
	if (pos == 0 && is_prev)
		return (NULL);
	return (curr_pipe);
}

void	handle_status(int status, t_tools *tools)
{
	if (WIFEXITED(status))
		tools->exit_status = WEXITSTATUS(status);
	if (tools && tools->exit_status == 127)
		printf("%s: command not found\n", tools->command->args[0]);
	if (g_signal == S_SIGINT_CMD)
		tools->exit_status = 130;
	g_signal = S_BASE;
}
