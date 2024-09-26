/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:22 by icseri            #+#    #+#             */
/*   Updated: 2024/09/26 13:54:08 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	execute(t_var *data)
{
	t_exec	*temp;
	
	temp = data->exec;
	if (temp == NULL)
		return ;
	if (temp->next == NULL)
		only_one_sequence(data, temp);
	else
	{
		first_sequence(data, temp);
		temp = temp->next;
		while (temp->next != NULL)
		{
			middle_sequence(data, temp);
			temp = temp->next;
		}
		if (temp->next == NULL)
			last_sequence(data, temp);
	}
}

void	only_one_sequence(t_var *data, t_exec *exec)
{
	if (is_builtin(exec->data) == true)
		exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
	else
	{
		data->pid = fork();
		if (data->pid == -1)
			safe_exit(data, FORK_FAIL);
		signals.child_pid = data->pid;
		if (data->pid == 0)
			exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
		else
		{
			waitpid(data->pid, &data->exit_status, 0);
			signals.child_pid = -1;
			if (WIFEXITED(data->exit_status))
				data->exit_code = WEXITSTATUS(data->exit_status);
			//printf("This is for debugging! exit code: %d\n", data->exit_code);
		}
	}
}

void	first_sequence(t_var *data, t_exec *exec)
{
	pipe(data->pipe1_fd);
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		close(data->pipe1_fd[0]);
		exec_sequence(data, exec, STDIN_FILENO, data->pipe1_fd[1]);
		safe_exit(data, EXIT_SUCCESS);
	}
}

void	middle_sequence(t_var *data, t_exec *exec)
{
	pipe(data->pipe2_fd);
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		close(data->pipe1_fd[1]);
		close(data->pipe2_fd[0]);
		exec_sequence(data, exec, data->pipe1_fd[0], data->pipe2_fd[1]);
		safe_exit(data, EXIT_SUCCESS);
	}
	else
	{
		close(data->pipe1_fd[0]);
		close(data->pipe1_fd[1]);
		data->pipe1_fd[0] = data->pipe2_fd[0];
		data->pipe1_fd[1] = data->pipe2_fd[1];
	}
}

void	last_sequence(t_var *data, t_exec *exec)
{
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		close(data->pipe1_fd[1]);
		exec_sequence(data, exec, data->pipe1_fd[0], STDOUT_FILENO);
		safe_exit(data, EXIT_SUCCESS);
	}
	else
	{
		close(data->pipe1_fd[0]);
		close(data->pipe1_fd[1]);
		wait(NULL);
		waitpid(data->pid, &data->exit_status, 0);
		if (WIFEXITED(data->exit_status))
			data->exit_code = WEXITSTATUS(data->exit_status);
		//printf("This is for debugging nexit code: %d\n", data->exit_code);
	}
}

void	exec_sequence(t_var *data, t_exec *exec, int read, int write)
{
	create_cmd_list(data, exec);
	if (is_builtin(data->cmd_list[0])	
		&& ft_strncmp("exit", data->cmd_list[0], 5) != 0 && write == STDOUT_FILENO)
	{
		data->stdout_copy = dup(STDOUT_FILENO);
		if (data->stdout_copy == -1)
			safe_exit(data, DUP2_FAIL);
	}
	if (redirect_in(data, exec) == false)
		return;
	safe_dup2(read, STDIN_FILENO, data);
	safe_dup2(write, STDOUT_FILENO, data);
	if (redirect_out(data, exec) == false)
		return;
	if (data->cmd_list != NULL)
	{
		if (exec_builtin(data) == false)
			exec_command(data);
	}
	safe_close(read, data);
	safe_close(write, data);
	safe_dup2(data->stdout_copy, STDOUT_FILENO, data);

}

void	exec_command(t_var *data)
{
	char	*abs_cmd;
	
	if (data->cmd_list[0] == NULL)
		return ;
	if (ft_strchr(data->cmd_list[0], '/') != NULL
		&& execve(data->cmd_list[0], data->cmd_list, data->env) == -1)
	{
		print_error(3, "minishell: ", data->cmd_list[0], ": No such file or directory");
		safe_exit(data, COMMAND_NOT_FOUND);
	}
	abs_cmd = get_abs_cmd(data, data->cmd_list[0]);
	if (abs_cmd == NULL)
	{
		print_error(3, "minishell: ", data->cmd_list[0], ": command not found");
		safe_exit(data, COMMAND_NOT_FOUND);
	}
	if (execve(abs_cmd, data->cmd_list, data->env) == -1)
	{
		print_error(3, "minishell: ", abs_cmd, ": No such file or directory");
		ft_free(&abs_cmd);
		safe_exit(data, COMMAND_NOT_FOUND);
	}
}

void	create_cmd_list(t_var *data, t_exec *exec)
{
	int		i;
	t_exec	*temp;

	//I have to extend the variables and remove the quotes here
	temp = exec;
	i = 0;
	while (temp != NULL && temp->type == WORD)
	{
		i++;
		temp = temp->down;
	}
	data->cmd_list = malloc(sizeof(char *) * (i + 1));
	if (!data->cmd_list)
		safe_exit(data, MALLOC_FAIL);
	temp = exec;
	i = 0;
	while (temp != NULL && temp->type == WORD)
	{
		data->cmd_list[i] = ft_strdup(exec->data);
		if (!data->cmd_list[i])
			safe_exit(data, MALLOC_FAIL);
		i++;
		temp = temp->down;
	}
	data->cmd_list[i] = NULL;
}

char	**get_paths(t_var *data)
{
	int		i;
	char	**path;

	i = 0;
	while (data->env[i] != NULL)
	{
		if (ft_strncmp(data->env[i], "PATH=", 5) == 0)
		{
			path = ft_split(data->env[i] + 5, ':');
			if (!path)
				safe_exit(data, MALLOC_FAIL);
			return (path);
		}
		i++;
	}
	return (NULL);
}

char	*get_abs_cmd(t_var *data, char *cmd)
{
	int		i;
	char	**path;
	char	*tmp;
	char	*path_cmd;

	path = get_paths(data);
	if (!path)
		return (cmd);
	i = 0;
	while (path[i] != NULL)
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			safe_exit(data, MALLOC_FAIL);
		path_cmd = ft_strjoin(tmp, cmd);
		ft_free(&tmp);
		if (!path_cmd)
			safe_exit(data, MALLOC_FAIL);
		if (access(path_cmd, F_OK) == 0)
		{
			free_array(path);
			return (path_cmd);
		}
		ft_free(&path_cmd);
		i++;
	}
	free_array(path);
	return (NULL);
}
