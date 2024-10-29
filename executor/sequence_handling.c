/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 10:31:10 by pvass             #+#    #+#             */
/*   Updated: 2024/10/29 15:29:55 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	only_one_sequence(t_var *data, t_exec *exec)
{
	heredoc(data, exec);
	create_cmd_list(data, exec);
	if (!data->cmd_list || !*data->cmd_list || is_builtin(data->cmd_list[0]))
		exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
	else
	{
		data->proc_count++;
		data->pid = fork();
		if (data->pid == -1)
		{
			print_error(1, "minishell: fork failed");
			safe_exit(data, FORK_FAIL);
		}
		if (data->pid == 0)
		{
			exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
			safe_exit(data, data->exit_code);
		}
		else
		{
			waitpid(data->pid, &data->exit_status, 0);
			if (WIFEXITED(data->exit_status))
				data->exit_code = WEXITSTATUS(data->exit_status);
		}
	}
}

void	first_sequence(t_var *data, t_exec *exec)
{
	heredoc(data, exec);
	create_cmd_list(data, exec);
	pipe(data->pipe1_fd);
	data->proc_count++;
	data->pid = fork();
	if (data->pid == -1)
	{
		print_error(1, "minishell: fork failed");
		safe_exit(data, FORK_FAIL);
	}
	if (data->pid == 0)
	{
		safe_close(&data->pipe1_fd[0]);
		exec_sequence(data, exec, STDIN_FILENO, data->pipe1_fd[1]);
		safe_exit(data, data->exit_code);
	}
}

void	middle_sequence(t_var *data, t_exec *exec)
{
	heredoc(data, exec);
	create_cmd_list(data, exec);
	pipe(data->pipe2_fd);
	data->proc_count++;
	data->pid = fork();
	if (data->pid == -1)
	{
		print_error(1, "minishell: fork failed");
		safe_exit(data, FORK_FAIL);
	}
	if (data->pid == 0)
	{
		safe_close(&data->pipe1_fd[1]);
		safe_close(&data->pipe2_fd[0]);
		exec_sequence(data, exec, data->pipe1_fd[0], data->pipe2_fd[1]);
		safe_exit(data, data->exit_code);
	}
	else
	{
		safe_close(&data->pipe1_fd[0]);
		safe_close(&data->pipe1_fd[1]);
		data->pipe1_fd[0] = data->pipe2_fd[0];
		data->pipe1_fd[1] = data->pipe2_fd[1];
	}
}

void	last_sequence(t_var *data, t_exec *exec)
{
	heredoc(data, exec);
	create_cmd_list(data, exec);
	data->proc_count++;
	data->pid = fork();
	if (data->pid == -1)
	{
		print_error(1, "minishell: fork failed");
		safe_exit(data, FORK_FAIL);
	}
	if (data->pid == 0)
	{
		safe_close(&data->pipe1_fd[1]);
		exec_sequence(data, exec, data->pipe1_fd[0], STDOUT_FILENO);
		safe_exit(data, data->exit_code);
	}
	else
	{
		safe_close(&data->pipe1_fd[0]);
		safe_close(&data->pipe1_fd[1]);
		waitpid(data->pid, &data->exit_status, 0);
		if (WIFEXITED(data->exit_status))
			data->exit_code = WEXITSTATUS(data->exit_status);
		while (--data->proc_count > 0)
			wait(NULL);
	}
}

void	exec_sequence(t_var *data, t_exec *exec, int read_fd, int write_fd)
{
	if (redirs_exist(data, exec) == false)
		return ;
	if (redirect_in(data, exec, &read_fd) == false)
		return ;
	if (redirect_out(data, exec, &write_fd) == false)
		return ;
	if (data->cmd_list != NULL)
	{
		if (exec_builtin(data) == false)
			exec_command(data);
	}
	safe_close(&write_fd);
	safe_close(&read_fd);
	safe_close(&data->fd_to_write);
	data->fd_to_write = STDOUT_FILENO;
}
