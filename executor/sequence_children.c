/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence_children.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:05:18 by pvass             #+#    #+#             */
/*   Updated: 2024/11/14 12:20:39 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	only_one_seq_child(t_var *data, t_exec *exec)
{
	sig_hand(CHILD);
	exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
	safe_exit(data, data->exit_code);
}

void	last_sequence_child(t_var *data, t_exec *exec)
{
	sig_hand(CHILD);
	safe_close(&data->pipe1_fd[1]);
	exec_sequence(data, exec, data->pipe1_fd[0], STDOUT_FILENO);
	safe_exit(data, data->exit_code);
}

void	check_status(t_var *data)
{
	if (WIFEXITED(data->exit_status))
		data->exit_code = WEXITSTATUS(data->exit_status);
	else if (WIFSIGNALED(data->exit_status))
	{
		data->exit_code = WTERMSIG(data->exit_status) + 128;
		if (data->exit_code == 128 + SIGINT)
			write(STDERR_FILENO, "\n", 1);
		else if (WCOREDUMP(data->exit_status))
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	}
}
