/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:02:06 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/08 14:33:52 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// static int	handle_heredoc(t_tools *tools, t_redir *redir)
// {
// 	char	*line;
// 	int		pipe_fd[2];
// 	char	*str_err;
// 	char	*str_err2;

// 	(void)tools; // No se usa en esta función por ahora, para evitar warnings
// 	str_err = "bash: warning: here-document delimited by end-of-file";
// 	str_err2 = " (wanted `END')";
// 	signal(SIGINT, SIG_DFL);
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe");
// 		return (-1);
// 	}
// 	g_signal = S_HEREDOC;
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (line == NULL)
// 		{
// 			ft_putstr_fd(str_err, STDOUT_FILENO);
// 			ft_putendl_fd(str_err2, STDOUT_FILENO);
// 			break ;
// 		}
// 		if (g_signal == S_CANCEL_EXEC)
//         {
// 			tools->exit_status = 130;
//             free(line);
//             break;
//         }
// 		if (strcmp(line, redir->file) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		//expansion de variables
// 		free(tools->arg_str);
// 		tools->arg_str = ft_strdup(line);
// 		tools->arg_str = expansor(tools);
// 		write(pipe_fd[1], tools->arg_str, strlen(tools->arg_str));
// 		write(pipe_fd[1], "\n", 1);
// 		free(line);
// 		// free(expanded_line);
// 	}
// 	signal(SIGINT, sigint_handler);
// 	g_signal = S_BASE;
// 	close(pipe_fd[1]);
// 	redir->fd = pipe_fd[0];
// 	return (1);
// }

static int preprocess_heredoc(t_tools *tools, const char *delimiter)
{
	char *line;
	int pipe_fd[2];

	signal(SIGINT, SIG_DFL);
	// Crear un pipe para redirigir el contenido del heredoc
	if (pipe(pipe_fd) == -1)
	{
	    perror("pipe");
	    return -1;
	}

	// Leer líneas hasta encontrar el delimitador
	while (1)
	{
		line = readline("> ");
		if (line == NULL || strcmp(line, delimiter) == 0)
		{
			ft_putstr_fd("bash: warning: here-document delim", STDOUT_FILENO);
			ft_putendl_fd("ited by end-of-file (wanted `END')", STDOUT_FILENO);
			free(line);
			break;
		}
		if (g_signal == S_CANCEL_EXEC)
		{
			tools->exit_status = 130;
			free(line);
			break;
		}

		// Aplicar expansor a la línea
		free(tools->arg_str); // Liberar cualquier valor previo
		tools->arg_str = ft_strdup(line);
		tools->arg_str = expansor(tools); // Expansión de variables

		// Escribir la línea expandida en el pipe
		write(pipe_fd[1], tools->arg_str, strlen(tools->arg_str));
		write(pipe_fd[1], "\n", 1);

		free(line);
	}
	signal(SIGINT, sigint_handler);
	g_signal = S_BASE;
	close(pipe_fd[1]); // Cerrar el extremo de escritura
	return (pipe_fd[0]); // Retornar el extremo de lectura
}

static int	handle_file_open_output(t_tools *tools, t_redir *redir)
{
	int	mode;

	(void)tools; // No se usa en esta función por ahora, para evitar warnings
	mode = O_CREAT | O_RDWR;
	if (redir->type == 3)
		mode = mode | O_APPEND;
	redir->fd = open(redir->file, mode, S_IRGRP | S_IWUSR | S_IRUSR | S_IROTH);
	return (redir->fd);
}

static int	get_open_mode(t_tools *tools, t_redir *redir)
{
	(void)tools; // No se usa en esta función por ahora, para evitar warnings
	if (redir->type == 0 || redir->type == 2)
		return (R_OK);
	else
		return (R_OK | W_OK);
}

static int	open_existing_file(t_tools *tools, t_redir *redir)
{
	int	mode;

	mode = get_open_mode(tools, redir);
	if (access(redir->file, mode) != -1)
	{
		if (redir->type == 3)
			mode = O_RDWR | O_APPEND;
		redir->fd = open(redir->file, mode);
		if (redir->fd == -1)
			return(-1);
		return (redir->fd);
	}
	else
	{
		printf("Minishell: %s: Permission denied\n", redir->file);
		return (-1);
	}
}

// int	file_open(t_tools *tools, t_redir *redir)
// {
// 	if (access(redir->file, F_OK) != -1)
// 		return (open_existing_file(tools, redir));
// 	else if (redir->type == 1 || redir->type == 3)
// 		return (handle_file_open_output(tools, redir));
// 	else if (redir->type == 2)
// 		return (handle_heredoc(tools, redir));
// 	else
// 	{
// 		printf("Minishell: %s: File or directory does not exist\n",
// 			redir->file);
// 		return (0);
// 	}
// 	return (1);
// }
int file_open(t_tools *tools, t_redir *redir)
{
    // Caso: Heredoc (procesado en un paso previo)
    if (redir->type == 2) // Heredoc
    {
        redir->fd = preprocess_heredoc(tools, redir->file);
        if (redir->fd == -1)
        {
            fprintf(stderr, "Error al procesar heredoc\n");
            return -1;
        }
        return redir->fd;
    }

    // Caso: Archivo existente
    if (access(redir->file, F_OK) != -1)
        return open_existing_file(tools, redir);

    // Caso: Redirección de salida o append
    if (redir->type == 1 || redir->type == 3)
        return handle_file_open_output(tools, redir);

    // Caso: Archivo no existe
    fprintf(stderr, "Minishell: %s: File or directory does not exist\n", redir->file);
    return 0;
}
