/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:16:37 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/08 13:58:27 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

void	print_ctrl(char *ctrl)
{
	if (ctrl)
		ft_putstr_fd(ctrl, 1);
	ft_putstr_fd("\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}

static void	sigint_handler_aux(void)
{
	if (g_signal == S_HEREDOC_END)
	{
		ft_putstr_fd("\n", 1);
		g_signal = S_CANCEL_EXEC;
	}
	else if (g_signal == S_HEREDOC)
	{
		print_ctrl(NULL);
		rl_redisplay();
		g_signal = S_CANCEL_EXEC;
		return ;
	}
	else if (g_signal == S_QUOTE)
	{
		print_ctrl("^C");
		g_signal = S_BASE;
		return ;
	}
}

void	sigint_handler(int sig)
{
	(void)sig;
	if (g_signal == S_BASE)
	{
		print_ctrl(NULL);
		rl_redisplay();
	}
	else if (g_signal == S_SIGINT)
	{
		print_ctrl(NULL);
	}
	else if (g_signal == S_CMD)
	{
		g_signal = S_SIGINT_CMD;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
	}
	else if (g_signal == S_MINI)
	{
		g_signal = S_SIGINT_CMD;
		rl_on_new_line();
	}
	// else
		// signal(SIGQUIT, SIG_IGN);
	sigint_handler_aux();
}

/// @brief Initialize signal handling.
/// This function sets up initial values for signal handling.
/// SIGINT : Ctrl+C
/// SIGQUIT : Ctrl+ \ //
/// SIG_IGN : ignorar signales
/// @return Void.
void	signal_init(void)
{
	g_signal = S_BASE;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
