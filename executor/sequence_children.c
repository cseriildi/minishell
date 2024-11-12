/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence_children.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:05:18 by pvass             #+#    #+#             */
/*   Updated: 2024/10/29 21:06:07 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	only_one_seq_child(t_var *data, t_exec *exec)
{
	sig_setup(SIGINT, SIG_DEFAULT);
	sig_setup(SIGQUIT, SIG_STANDARD);
	exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
	safe_exit(data, data->exit_code);
}

void	last_sequence_child(t_var *data, t_exec *exec)
{
	sig_setup(SIGINT, SIG_DEFAULT);
	sig_setup(SIGQUIT, SIG_STANDARD);
	safe_close(&data->pipe1_fd[1]);
	exec_sequence(data, exec, data->pipe1_fd[0], STDOUT_FILENO);
	safe_exit(data, data->exit_code);
}
