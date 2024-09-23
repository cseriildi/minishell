/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:22 by icseri            #+#    #+#             */
/*   Updated: 2024/09/23 16:42:09 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	execute(t_var *data)
{
	if (data->exec == NULL)
		return ;
	if (data->exec->next == NULL)
		only_one_sequence(data, data->exec);
	else
	{
		first_sequence(data, data->exec);
		data->exec = data->exec->next;
		while (data->exec->next != NULL)
		{
			middle_sequence(data, data->exec);
			data->exec = data->exec->next;
		}
		if (data->exec->next == NULL)
			last_sequence(data, data->exec);
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
		if (data->pid == 0)
			exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
		else
		{
			waitpid(data->pid, &data->exit_status, 0);
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
		exit(EXIT_SUCCESS);
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
		exit(EXIT_SUCCESS);
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
		exit(EXIT_SUCCESS);
	}
	else
	{
		wait(&data->exit_status);
		waitpid(data->pid, &data->exit_status, 0);
		if (WIFEXITED(data->exit_status))
			data->exit_code = WEXITSTATUS(data->exit_status);
		//printf("This is for debugging nexit code: %d\n", data->exit_code);
	}
}

void	exec_sequence(t_var *data, t_exec *exec, int read, int write)
{
	char	**cmd_list;
	t_exec	*temp;
	int		out_fd;

	temp = exec;
	cmd_list = create_cmd_list(data, exec);
	while (temp != NULL && temp->type == WORD)
		temp = temp->down;
	temp = exec;
	out_fd = dup(STDOUT_FILENO);
	if (out_fd == -1)
		safe_exit(data, DUP2_FAIL);
	while (temp != NULL)
	{
		redirect(data, temp);
		temp = temp->down;
	}
	dup_pipes(data, read, write);
	if (cmd_list != NULL)
	{
		if (exec_builtin(data, cmd_list) == false)
			exec_command(data, cmd_list);
	}
	if (read != STDIN_FILENO)
		close (read);
	if (write != STDOUT_FILENO)
		close (write);
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		close(out_fd);
		safe_exit(data, DUP2_FAIL);
	}
	close(out_fd);

}

void	redirect(t_var *data, t_exec *exec)
{
	int	fd;
	
	if (exec->type == RED_IN)
	{
		fd = open(exec->data, O_RDONLY);
		if (fd == -1)
		{
			print_error(4, "minishell: ", exec->data, ": ", strerror(errno));
			data->exit_code = CANNOT_OPEN_FILE;
			return ;
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			safe_exit(data, DUP2_FAIL);
		}
		close(fd);
	}
	else if (exec->type == RED_OUT)
	{
		fd = open(exec->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			print_error(4, "minishell: ", exec->data, ": ", strerror(errno));
			data->exit_code = CANNOT_OPEN_FILE;
			return ;
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			safe_exit(data, DUP2_FAIL);
		}
		close(fd);
	}
	else if (exec->type == APPEND)
	{
		fd = open(exec->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			print_error(4, "minishell: ", exec->data, ": ", strerror(errno));
			data->exit_code = CANNOT_OPEN_FILE;
			return ;
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			safe_exit(data, DUP2_FAIL);
		}
		close(fd);
	}
	else if (exec->type == HERE_DOC)
	{
		//I have to implement here_doc
		//Do NOT forget about quotes in the LIMITER
	}
}

void	dup_pipes(t_var *data, int read, int write)
{
	if (read != STDIN_FILENO)
	{
		if (dup2(read, STDIN_FILENO) == -1)
			safe_exit(data, DUP2_FAIL);
		close(read);
	}
	if (write != STDOUT_FILENO)
	{
		if (dup2(write, STDOUT_FILENO) == -1)
			safe_exit(data, DUP2_FAIL);
		close(write);
	}
}

void	exec_command(t_var *data, char **cmd_list)
{
	char	*abs_cmd;
	
	if (cmd_list[0] == NULL)
		return ;
	if (ft_strchr(cmd_list[0], '/') != NULL
		&& execve(cmd_list[0], cmd_list, data->env) == -1)
	{
		print_error(3, "minishell: ", cmd_list[0], ": No such file or directory");
		safe_exit(data, COMMAND_NOT_FOUND);
	}
	abs_cmd = get_abs_cmd(data, cmd_list[0]);
	if (abs_cmd == NULL)
	{
		print_error(3, "minishell: ", cmd_list[0], ": command not found");
		safe_exit(data, COMMAND_NOT_FOUND);
	}
	if (execve(abs_cmd, cmd_list, data->env) == -1)
	{
		print_error(3, "minishell: ", abs_cmd, ": No such file or directory");
		//ft_free(&abs_cmd);
		safe_exit(data, COMMAND_NOT_FOUND);
	}
}

char	**create_cmd_list(t_var *data, t_exec *exec)
{
	int		i;
	char	**cmd_list;
	t_exec	*temp;

	//I have to extend the variables and remove the quotes here
	//Fix to work with relative path
	temp = exec;
	i = 0;
	while (temp != NULL && temp->type == WORD)
	{
		i++;
		temp = temp->down;
	}
	cmd_list = malloc(sizeof(char *) * (i + 1));
	if (!cmd_list)
		safe_exit(data, MALLOC_FAIL);
	temp = exec;
	i = 0;
	while (temp != NULL && temp->type == WORD)
	{
		cmd_list[i] = temp->data;
		i++;
		temp = temp->down;
	}
	cmd_list[i] = NULL;
	return (cmd_list);
}

bool	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) && ft_strncmp(cmd, "cd", 3)
		&& ft_strncmp(cmd, "pwd", 4) && ft_strncmp(cmd, "export", 7)
		&& ft_strncmp(cmd, "unset", 5) && ft_strncmp(cmd, "env", 4)
		&& ft_strncmp(cmd, "exit", 5))
		return (false);
	return (true);
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
