/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:00:39 by pvass             #+#    #+#             */
/*   Updated: 2024/09/25 13:06:03 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void handle_sigint(int sig) 
{
	(void)sig;
    if (signals.child_pid > 0) {

        //ioctl(STDIN_FILENO, TIOCSTI, "\n");
		ft_printf("\n");
		rl_on_new_line();
		kill(signals.child_pid, SIGINT);
    } 
	else 
	{
		if (signals.interactive == 1)
		{
			ioctl(STDIN_FILENO, TIOCSTI, "\n");
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
		{
			ft_printf("\n");
			rl_on_new_line();
		}
    }
}

void handle_sigquit(int sig) 
{
	(void) sig;
}

void	init_signals()
{
	struct sigaction sa_int, sa_quit;
	
	sa_int.sa_handler = &handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART; 
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
        perror("sigaction(SIGINT) failed");
        exit(1);
    }
	
	sa_quit.sa_handler = &handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
        perror("sigaction(SIGQUIT) failed");
        exit(1);
    }
}