/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 10:42:59 by migonzal          #+#    #+#             */
/*   Updated: 2025/04/30 14:15:10 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Resets the structure containing execution tools.
/// Frees allocated memory, clears stored data, and reinitializes the structure.
/// @param tools Pointer to the tools structure being reset.
/// @return Always returns 1 after successful reset.
int	reset_tools(t_tools *tools)
{
	free_command(tools->command);
	free(tools->arg_str);
	tools->arg_str = NULL;
	ft_free_arr(tools->paths);
	tools->paths = NULL;
	if (init_tools(tools) == 0)
		return (0);
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
	tools->arg_str = NULL;
	tools->reset = 0;
	tools->command = init_command();
	if (parse_envp(tools) == 0)
		return (0);
	return (1);
}

int	minishell_loop(t_tools *tools) //toda la gestion de squote y dquote estan por revisar, y todo lo que tenga que ver con las señales hay que prenderle fuego y empezzar de 0
{
	char	*aux;
	int		in_dquote;
	int		in_squote;

	in_dquote = 0;
	in_squote = 0;
	while (1)
	{
		if (g_signal == S_SIGINT)
			tools->exit_status = 1;
		// la funcion imprime una invitacion. Si nada se lee, se cierra el programma.
		tools->arg_str = readline("minishell? "); // Aqui inicia el readline
		if (!tools->arg_str)
		{
			ft_putendl_fd("No line read, exit minishell", STDOUT_FILENO);
			exit(EXIT_SUCCESS);
		}
		// el commando se lee y se "strim" para quitar los espacios.
		aux = ft_strtrim(tools->arg_str, " ");
		free(tools->arg_str);
		tools->arg_str = aux;
		if (tools->arg_str[0] == '\0')  //si el parseo recibe un string vacio reset de la mini
		{
			reset_tools(tools);
			continue ;
		}
		// Si el comando es invalido con los pipes, se reeinicia.
		if (!validate_pipes(tools->arg_str))   // Intento de solucionar un error con las pipes, a revisar porque el error es mas profundo y no afecta solo a las pipe
		{
			reset_tools(tools);
			continue ;
		}
		if (check_quotes(in_dquote, in_squote, tools) == 0)
		{
			reset_tools(tools);
			continue ;
		}
		add_history(tools->arg_str);   // gestion del historial
		expansor(tools);               //gestion de la expansion de variables
		if (tools->command)
			free_command(tools->command);
		tools->command = parser(tools->arg_str);   //Aqui parseo el string que recibe la funcion readline para pasarselo al executor
		if (!tools->command)
			return (ft_clean_all(tools), 0);
		executor(tools);               // gestion del executor
		reset_tools(tools);				//cuando acaba todo, reset de todo y se queda listo para recibir el sig comando
	}
	return (1);
}

void	ft_clean_all(t_tools *tools)
{
	if (tools->arg_str)
		free(tools->arg_str);
	if (tools->command)
		free_command(tools->command);
	if (tools->pwd)
		free(tools->pwd);
	if (tools->old_pwd)
		free(tools->old_pwd);
	if (tools->envp)
		ft_free_arr(tools->envp);
	ft_free_arr(tools->paths);
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
