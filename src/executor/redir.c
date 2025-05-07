/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:04:01 by sperez-s          #+#    #+#             */
/*   Updated: 2025/05/07 10:23:37 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	fd_redir(t_redir *redir)
// {
// 	if (redir->type == 0 || redir->type == 2)
// 		dup2(redir->fd, STDIN_FILENO);
// 	else
// 		dup2(redir->fd, STDOUT_FILENO);
// 	close(redir->fd);
// }

// // / @brief Prepara las redirecciones para un comando abriendo los archivos
// // / necesarios y estableciendo los descriptores de fichero apropiados.
// // / @param comando
// // / @return 0 si las redirecciones se han configurado correctamente, sino -1
// int	redir_setup(t_command *command)
// {
// 	t_redir	*curr_redir;
// 	t_redir	*first_redir;

// 	curr_redir = command->redir;
// 	first_redir = command->redir;
// 	while (curr_redir)
// 	{
// 		if (file_open(curr_redir) < 1)
// 			return (-1);
// 		curr_redir = curr_redir->next;d
// 	}
// 	curr_redir = first_redir;
// 	while (curr_redir)
// 	{
// 		fd_redir(curr_redir);
// 		curr_redir = curr_redir->next;
// 	}
// 	return (0);
// }


static void	fd_redir(t_tools *tools, t_redir *redir, int no_command)
{
	int fd = 0;

	(void)tools; // No se usa en esta función por ahora, para evitar warnings
	// Abrir el archivo según el tipo de redirección
	if (redir->type == 0 || redir->type == 2) // Redirección de entrada (< y <<)
	{
		if (dup2(redir->fd, STDIN_FILENO) == -1)
		{
			perror("dup2 failed for STDIN");
			close(redir->fd);
			return;
		}
	}
	else // Redirección de salida (> y >>)
	{
		if (no_command)
		{
			// Caso especial: No hay comando, solo crear/truncar el archivo
			if (redir->type == 1) // `>` para truncar el archivo
				fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (redir->type == 3) // `>>` para añadir al archivo
				fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);

			if (fd == -1)
			{
				perror("Error opening file");
				return;
			}
			close(fd); // Cerrar el archivo inmediatamente
		}
		else
		{
			// Redirección normal: Redirigir STDOUT al archivo
			if (dup2(redir->fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 failed for STDOUT");
				close(redir->fd);
				return;
			}
		}
	}

	// Cerrar el descriptor de archivo original
	if (close(redir->fd) == -1)
		perror("close failed");
}

/**
 * Configura las redirecciones para un comando abriendo los archivos
 * necesarios y ajustando los descriptores de archivo.
 *
 * @param tools Estructura global de herramientas.
 * @param command El comando para el cual se configura la redirección.
 * @return 0 si las redirecciones se configuran correctamente, -1 en caso de error.
 */
int	redir_setup(t_tools *tools, t_command *command)
{
	t_redir	*curr_redir;
	t_redir	*first_redir;

	curr_redir = command->redir;
	first_redir = command->redir;

	// Detectar si no hay un comando asociado
	int no_command = (command->args == NULL || command->args[0] == NULL);

	// Abrir y preparar los archivos para redirección
	while (curr_redir)
	{
		if (file_open(tools, curr_redir) < 1)
			return (-1);
		curr_redir = curr_redir->next;
	}
	curr_redir = first_redir;

	// Configurar los descriptores de archivo
	while (curr_redir)
	{
		fd_redir(tools, curr_redir, no_command); // Crear/truncar o redirigir
		curr_redir = curr_redir->next;
	}
	return (0);
}
