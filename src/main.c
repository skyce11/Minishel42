/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 10:42:59 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/17 17:06:07 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Initializes the tools structure for command execution.
/// Sets default values, initializes the command structure,
/// and processes environment variables.
/// @param tools Pointer to the tools structure to initialize.
/// @return Always returns 1 after successful initialization.
int	init_tools(t_tools *tools)
{
	tools->exit_status = 0;
	tools->arg_str = NULL;
	tools->paths = NULL;
	tools->pwd = NULL;
	tools->old_pwd = NULL;
	tools->reset = 0;
	tools->command = init_command();
	parse_envp(tools);
	return (1);
}

static void	preprocess_redirs_fill(const char *input, char *out, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (input[i] == '<' || input[i] == '>')
		{
			if (j > 0 && out[j - 1] != ' ')
				out[j++] = ' ';
			out[j++] = input[i];
			if (input[i + 1] == input[i])
			{
				out[j++] = input[i + 1];
				i++;
			}
			if (input[i + 1] && input[i + 1] != ' ')
				out[j++] = ' ';
		}
		else
			out[j++] = input[i];
		i++;
	}
	out[j] = '\0';
}

static char	*preprocess_redirs(const char *input)
{
	size_t	len;
	char	*out;

	len = 0;
	out = NULL;
	len = ft_strlen(input);
	out = malloc(len * 3 + 1);
	if (!out)
		return (NULL);
	preprocess_redirs_fill(input, out, len);
	return (out);
}

int	minishell_loop(t_tools *tools)
{
	char	*preprocessed;

	g_signal = 0;
	while (1)
	{
		tools->arg_str = readline("minishell$ ");
		if (g_signal == 2)
			tools->exit_status = 130;
		if (!tools->arg_str)
			return (ft_putstr_fd("exit\n", 1), ft_clean_all(tools), 0);
		ft_strim_without_leaks(tools);
		add_history(tools->arg_str);
		preprocessed = preprocess_redirs(tools->arg_str);
		if (!preprocessed)
			return (ft_clean_all(tools), 0);
		free(tools->arg_str);
		tools->arg_str = preprocessed;
		expansor(tools);
		parser(tools);
		if (!tools->command)
			return (ft_clean_all(tools), 0);
		executor(tools);
		reset_tools(tools);
	}
	return (1);
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
	init_tools(&tools);
	update_shlvl(&tools);
	signal_init();
	if (find_pwd(&tools) == 0)
		return (ft_clean_all(&tools), 0);
	printf("AQUI EMPIEZA LA MINISHELL\n");
	if (minishell_loop(&tools) == 0)
		return (rl_clear_history(), 0);
	rl_clear_history();
	ft_clean_all(&tools);
	return (0);
}
