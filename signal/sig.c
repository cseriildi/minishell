/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:44:14 by pvass             #+#    #+#             */
/*   Updated: 2024/11/06 15:08:25 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	sig_setup(int signo, t_sig state)
{
	struct sigaction	sa;

	if(state == SIG_STANDARD)
	{
		sa.sa_flags = SA_RESTART | SA_SIGINFO;
		sa.sa_sigaction = handle_signal_std;
	}
	else if (state == SIG_DEFAULT)
	{
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = SIG_DFL;
	}
	else if (state == SIG_IGNORE)
	{
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = SIG_IGN;
	}
	if (sigaction(signo, &sa, NULL) != 0)
		print_error(1, "The signal is not supported");
}

bool	signal_homemade_check(t_var *data)
{
	char	*temp;

	if (data->cmd_list == NULL || data->cmd_list[0] == NULL)
		return (FALSE);
	temp = ft_strrchr(data->cmd_list[0], '/');
	if (temp == NULL)
		return (FALSE);
	if (ft_strncmp("/minishell", temp, 11) == 0)
		return (TRUE);
	return (FALSE);
}

void	handle_signal_util_1(int signo, t_var *data)
{
	if (signo == SIGINT)
	{
		if (data->is_heredoc == TRUE)
			data->is_heredoc = FALSE;
		if (data->pid != 0 && (!data->cmd_list
				|| signal_homemade_check(data) == 0))
		{
			if (data->proc_count == 0)
				ioctl(STDIN_FILENO, TIOCSTI, "\n");
			else
				write(STDERR_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
	}
}

void	handle_signal_util_2(int signo, t_var *data)
{
	if (signo == SIGQUIT)
	{
		if (data->pid != 0 && data->proc_count != 0
			&& signal_homemade_check(data) == 0)
		{
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
	}
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
	if (data->proc_count == 0)
		data->exit_code = 128 + signo;
	handle_signal_util_1(signo, data);
	handle_signal_util_2(signo, data);
	if (signo == SIGUSR1)
		safe_exit(data, data->exit_code);
	else if ((signo == SIGTERM && data->pid != 0)
		|| signo == SIGPIPE)
		safe_exit(data, 128 + signo);
}
