/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:44:14 by pvass             #+#    #+#             */
/*   Updated: 2024/10/29 17:17:04 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	setup_signal(int signo, t_sig state)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	if (state == SIG_DEFAULT || state == SIG_IGNORE)
	{
		sa.sa_flags = SA_RESTART;
		if (state == SIG_DEFAULT)
			sa.sa_handler = SIG_DFL;
		else if (state == SIG_IGNORE)
			sa.sa_handler = SIG_IGN;
	}
	else
	{
		sa.sa_flags = SA_RESTART | SA_SIGINFO;
		if (state == SIG_STANDARD)
		{
			sa.sa_sigaction = handle_signal_std;
		}
	}
	if (sigaction(signo, &sa, NULL) != 0)
		print_error(1, "The signal is not supported");
}

void	handle_signal_std(int signo, siginfo_t *info, void *context)
{
	static t_var	*data;

	(void)info;
	if (!data)
	{
		data = context;
		return ;
	}
	data->exit_code = 128 + signo;
	printf("HALOOOOOOOOOOO\n\n\n\n");
	if (signo == SIGINT /* && data->has_child == 0 */)
	{
		if (data->is_heredoc == TRUE)
			data->is_heredoc = FALSE;
		if (data->proc_count == 0)
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else if (signo == SIGUSR1)
		safe_exit(data, data->exit_code);
	else if ((signo == SIGTERM && data->pid != 0) || \
		signo == SIGPIPE)
		safe_exit(data, 128 + signo);
}
