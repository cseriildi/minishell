/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:00:39 by pvass             #+#    #+#             */
/*   Updated: 2024/10/01 13:19:39 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sigint(int sig)
{
	signals.data->exit_code = 128 + sig;
	if (signals.interactive == 1)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	handle_sigquit(int sig)
{
	(void) sig;
}

void	init_signals(t_var *data)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	signals.data = data;
	sa_int.sa_handler = &handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("sigaction(SIGINT) failed");
		safe_exit(data, data->exit_code);
	}
	//data->exit_code = signals.exit_c;
	sa_quit.sa_handler = &handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction(SIGQUIT) failed");
		safe_exit(data, 3);
	}
}
