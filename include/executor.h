/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 13:25:03 by sperez-s          #+#    #+#             */
/*   Updated: 2025/03/31 11:10:50 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <errno.h>
# include <unistd.h>

typedef struct s_pipe
{
	int				pipe[2];
	struct s_pipe	*next;
}	t_pipe;

typedef struct s_pipes_command
{
	t_pipe	*prev;
	t_pipe	*curr;
}	t_pipes_command;

#endif
