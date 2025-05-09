/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 10:42:59 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/09 20:41:54 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Resets the structure containing execution tools.
/// Frees allocated memory, clears stored data, and reinitializes the structure.
/// @param tools Pointer to the tools structure being reset.
/// @return Always returns 1 after successful reset.
int	reset_tools(t_tools *tools)
{
	int	last_exit_status;

	last_exit_status = tools->exit_status;
	signal_init();
	free_command(tools->command);
	if (tools->arg_str)
		free(tools->arg_str);
	tools->arg_str = NULL;
	if (tools->paths)
		ft_free_arr(tools->paths);
	tools->paths = NULL;
	init_tools(tools);
	tools->exit_status = last_exit_status;
	tools->reset = 1;
	return (1);
}

/// @brief Initializes the tools structure for command execution.
/// Sets default values, initializes the command structure,
/// and processes environment variables.
/// @param tools Pointer to the tools structure to initialize.
/// @return Always returns 1 after successful initialization.
int	init_tools(t_tools *tools)
{
	tools->exit_status = 0;
	tools->arg_str = NULL;
	tools->reset = 0;
	tools->command = init_command();
	if (parse_envp(tools) == 0)
		return (0);
	return (1);
}

int	minishell_loop(t_tools *tools)
{
	char	*aux;

	while (1)
	{
		tools->arg_str = readline("minishell ");
		if (!tools->arg_str)
			return (ft_putstr_fd("exit\n", 1), ft_clean_all(tools), 0);
		aux = ft_strtrim(tools->arg_str, " ");
		free(tools->arg_str);
		tools->arg_str = aux;
		if (tools->arg_str[0] == '\0' || !validate_pipes(tools->arg_str)
			|| check_quotes(0, 0, tools) == 0)
		{
			reset_tools(tools);
			continue ;
		}
		add_history(tools->arg_str);
		expansor(tools);
		parser(tools);
		if (!tools->command)
			return (ft_clean_all(tools), 0);
		executor(tools);
		reset_tools(tools);
	}
	return (1);
}

/// @brief update the nbr of SHLVL.
/// @param tools
/// @param flag if flags = 1 -> increment ; if flags = 0 -> decrement;
void	update_shlvl(t_tools *tools)
{
	int		i;
	int		shlvl;
	char	*new_val;
	char	*entry;

	i = 0;
	while (tools->envp[i++])
	{
		if (!ft_strncmp(tools->envp[i], "SHLVL=", 6))
		{
			shlvl = ft_atoi(tools->envp[i] + 6);
			shlvl++;
			new_val = ft_itoa(shlvl);
			entry = ft_strjoin("SHLVL=", new_val);
			free(new_val);
			free(tools->envp[i]);
			tools->envp[i] = entry;
			return ;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_tools	tools;

	if (argc != 1 || argv[1])
	{
		ft_putstr_fd("Este programa no acepta argumentos inútil\n", 1);
		return (0);
	}
	tools.envp = arrdup(envp);
	if (!tools.envp)
		return (ft_free_arr(tools.envp), 0);
	update_shlvl(&tools);
	signal_init();
	if (find_pwd(&tools) == 0)
		return (ft_clean_all(&tools), 0);
	if (init_tools(&tools) == 0)
		return (ft_clean_all(&tools), 0);
	printf("AQUI EMPIEZA LA MINISHELL\n");
	if (minishell_loop(&tools) == 0)
		return (1);
	ft_clean_all(&tools);
	return (0);
}
