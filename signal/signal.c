/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:44:14 by pvass             #+#    #+#             */
/*   Updated: 2024/11/13 09:38:54 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_main(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		g_sig_num = sig;
	}
}

void	sigint_here(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		g_sig_num = sig;
	}
}

void	sigint_in_cmd(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		g_sig_num = sig;
	}
}

void	sigquit_in_cmd(int sig)
{
	if (sig == SIGQUIT)
	{
		write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	sig_hand(int type)
{
	if (type == MAIN)
	{
		g_sig_num = 0;
		signal(SIGINT, sigint_main);
		signal(SIGQUIT, SIG_IGN);
	}
	if (type == HEREDOC)
	{
		signal(SIGINT, sigint_here);
		signal(SIGQUIT, SIG_IGN);
	}
	if (type == IN_COMMAND)
	{
		signal(SIGINT, sigint_in_cmd);
		signal(SIGQUIT, sigquit_in_cmd);
	}
}
