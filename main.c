/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/11/12 20:43:02 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_sig_num = 0; 

/* void	sigint_no_child(int sig)
{
	(void) sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	sigint_with_child(int sig)
{
	(void) sig;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	sigint_heredoc(int sig)
{
	(void) sig;
	rl_on_new_line();
	rl_replace_line("", 0);
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

void	handle_sigint(int sig, t_var *data)
{
	if (sig == SIGINT)
	{
		if (data->is_heredoc == TRUE)
			data->is_heredoc = FALSE;
		if (data->pid != 0 && (!data->cmd_list
				|| signal_homemade_check(data) == 0))
		{
			if (data->proc_count == 0)
				signal(SIGINT, sigint_no_child);
			else
				signal(SIGINT, sigint_with_child);
		}
	}
}

void	sigquit(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	handle_sigquit(int sig, t_var *data)
{
	if (sig == SIGQUIT)
	{
		if (data->pid != 0 && data->proc_count != 0
			&& signal_homemade_check(data) == 0)
				signal(SIGQUIT, sigquit);
	}
} */

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

int	main(void)
{
	t_var	*data;

	data = ft_calloc(sizeof(t_var), 1);
	if (!data)
		malloc_failed(data);
	init(data);
	while (true)
	{
		if (g_sig_num == SIGINT + 128)
			data->exit_code = 130;
		sig_hand(MAIN);
		read_input(data);
		lexer(data);
		if (data->missing_quote == true)
		{
			free_all(data);
			continue ;
		}
		parse(data);
		execute(data);
		free_all(data);
	}
}
