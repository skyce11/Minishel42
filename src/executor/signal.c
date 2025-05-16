/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:16:37 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/16 14:36:53 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void	print_ctrl(char *ctrl)
{
	if (ctrl)
		ft_putstr_fd(ctrl, 1);
	ft_putstr_fd("\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = 2;
	print_ctrl(NULL);
	rl_redisplay();
}

void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	g_signal = 2;
	ft_putstr_fd("\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	close(0);
}

void	signal_init(void)
{
	g_signal = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
