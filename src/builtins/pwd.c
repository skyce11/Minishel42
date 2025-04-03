/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:11:19 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 10:15:20 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_tools *tools)
{
	char	*pwd;

	tools->exit_status = 0;
	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, 1);
	ft_memfree(pwd);
}
