/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 10:42:59 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/03 19:05:24 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Resets the structure containing execution tools.
/// Frees allocated memory, clears stored data, and reinitializes the structure.
/// @param tools Pointer to the tools structure being reset.
/// @return Always returns 1 after successful reset.
int	reset_tools(t_tools *tools)
{
	signal_init();
	free_command(tools->command);
	free(tools->arg_str);
	tools->arg_str = NULL;
	ft_free_arr(tools->paths);
	tools->paths = NULL;
	init_tools(tools);
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
	parse_envp(tools);
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
		tools->command = parser(tools->arg_str);   //Aqui parseo el string que recibe la funcion readline para pasarselo al executor
		executor(tools);               // gestion del executor
		reset_tools(tools);				//cuando acaba todo, reset de todo y se queda listo para recibir el sig comando
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
		printf("Este programa no acepta argumentos inútil\n");
		exit(0);
	}
	tools.envp = arrdup(envp);   // guardo el enviroment antes de hacer nada
	update_shlvl(&tools);
	signal_init();   			// señales, hay que prederlo fuego a esto
	find_pwd(&tools); 			// estoy guardando el las variables PATH y OLDPATH
	init_tools(&tools);
	printf("AQUI EMPIEZA LA MINISHELL\n");
	minishell_loop(&tools);     //este es loop que mantiene abierta el prompt de la mini hasta que le digas que se cierre
	return (0);
}
