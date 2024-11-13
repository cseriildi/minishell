/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence_children.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:05:18 by pvass             #+#    #+#             */
/*   Updated: 2024/11/13 09:10:25 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	only_one_seq_child(t_var *data, t_exec *exec)
{
	sig_hand(IN_COMMAND);
	exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
	safe_exit(data, data->exit_code);
}

void	last_sequence_child(t_var *data, t_exec *exec)
{
	sig_hand(IN_COMMAND);
	safe_close(&data->pipe1_fd[1]);
	exec_sequence(data, exec, data->pipe1_fd[0], STDOUT_FILENO);
	safe_exit(data, data->exit_code);
}
