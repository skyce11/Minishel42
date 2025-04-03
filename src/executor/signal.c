/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:16:37 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 10:17:12 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

static void	sigint_handler_aux(void);

void	signal_init(void)
{
	g_signal = S_BASE;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sigint_handler(int sig)
{
	(void)sig;
	if (g_signal == S_BASE || g_signal == S_SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_signal == S_CMD)
	{
		g_signal = S_SIGINT_CMD;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
	}
	else if (g_signal == S_HEREDOC)
	{
		ft_putstr_fd("\n", 1);
		signal(SIGQUIT, sigquit_handler);
		exit(0);
	}
	else
		signal(SIGQUIT, SIG_IGN);
	sigint_handler_aux();
}

void	sigquit_handler(int sig)
{
	(void)sig;
	write (1, "HOLA\n", 5);
	ft_putstr_fd("Quit (core dumped)\n", 1);
	exit(131);
}

static void	sigint_handler_aux(void)
{
	if (g_signal == S_HEREDOC_END)
	{
		ft_putstr_fd("\n", 1);
		g_signal = S_CANCEL_EXEC;
	}
	else if (g_signal == S_BASE || g_signal == S_HEREDOC)
		g_signal = S_SIGINT;
}
