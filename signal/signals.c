/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:00:39 by pvass             #+#    #+#             */
/*   Updated: 2024/09/27 11:17:20 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void handle_sigint(int sig) 
{
	(void)sig;
    if (signals.child_pid > 0) 
	{
		ft_printf("\n");
		rl_on_new_line();
		//data->exit_code = 130
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
	(void) sa_int;
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
