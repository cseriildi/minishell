/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:22 by icseri            #+#    #+#             */
/*   Updated: 2024/10/02 15:23:58 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_in_out_app(t_exec *exec)
{
	if (exec->type == RED_IN || exec->type == RED_OUT || exec->type == APPEND)
		return (1);
	else 
		return (0);
}

bool good_redir_path(char *redir, t_var *data)
{
	char	*new;
	int		i;
	int		count;
	bool	res;

	count = 0;
	i = 0;
	while (redir[i] != '\0')
	{
		if (redir[i] == '/')
			count = i;
		i++;
	}
	new = ft_substr(redir, 0, i);
	if (new == NULL)
	{
		print_error(1, "minishell: malloc failed");
		safe_exit(data, MALLOC_FAIL);
	}
	res = is_directory(new);
	free(new);
	return (res);
}

bool	redirs_exist(t_var *data, t_exec *exec)
{
	t_exec	*temp2;

	temp2 = exec;
	while (temp2 != NULL)
	{
		if (is_in_out_app(temp2) == 1)
		{
			if (is_directory(temp2->data) == 1)
			{
				data->exit_code = 1;
				print_error(3, "minishell: ", temp2->data, ": Is a directory");
				return (0);
			}
			if (temp2->type != RED_IN && good_redir_path(temp2->data, data) == 0)
			{
				data->exit_code = 1;
				print_error(3, "minishell: ", temp2->data, ": Not a directory");
				return (0);
			}
			if (temp2->type == RED_IN && access(temp2->data, F_OK) == -1)
			{
				data->exit_code = 1;
				print_error(3, "minishell: ", temp2->data, ": No such file or directory");
				return (0);
			}
		}
		temp2 = temp2->down;
	}
	return (1);
}

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
	if (is_builtin(exec->data) == true || exec->type != WORD)
	{
		data->stdout_copy = dup(STDOUT_FILENO);
		exec_sequence(data, exec, STDIN_FILENO, STDOUT_FILENO);
		safe_dup2(&data->stdout_copy, STDOUT_FILENO, data);
	}
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
		safe_close(&data->pipe1_fd[0]);
		exec_sequence(data, exec, STDIN_FILENO, data->pipe1_fd[1]);
		safe_exit(data, data->exit_code);
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
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		safe_close(&data->pipe1_fd[1]);
		data->stdout_copy = dup(STDOUT_FILENO);
		exec_sequence(data, exec, data->pipe1_fd[0], STDOUT_FILENO);
		safe_dup2(&data->stdout_copy, STDOUT_FILENO, data);
		safe_exit(data, data->exit_code);
	}
	else
	{
		safe_close(&data->pipe1_fd[0]);
		safe_close(&data->pipe1_fd[1]);
		wait(NULL);
		waitpid(data->pid, &data->exit_status, 0);
		if (WIFEXITED(data->exit_status))
			data->exit_code = WEXITSTATUS(data->exit_status);
		//printf("This is for debugging nexit code: %d\n", data->exit_code);
	}
}

void	exec_sequence(t_var *data, t_exec *exec, int read_fd, int write_fd)
{
	create_cmd_list(data, exec);
	if (redirs_exist(data, exec) == 0)
	{
		safe_dup2(&read_fd, STDIN_FILENO, data);
		safe_dup2(&write_fd, STDOUT_FILENO, data);
		return ;
	}
	if (redirect_in(data, exec) == false)
		return ;
	safe_dup2(&read_fd, STDIN_FILENO, data);
	safe_dup2(&write_fd, STDOUT_FILENO, data);
	if (redirect_out(data, exec) == false)
		return ;
	if (data->cmd_list != NULL)
	{
		if (exec_builtin(data) == false)
			exec_command(data);
	}
}

int is_directory(const char *path)
{
    struct stat path_stat;

    // Get information about the file at 'path'
    if (stat(path, &path_stat) != 0) {
		//perror("stat");
        return 0;  // Error occurred
    }

    // Check if it is a directory
    return S_ISDIR(path_stat.st_mode);
}

void	exec_command(t_var *data)
{
	char	*cmd;
	char	*abs_cmd;

	cmd = data->cmd_list[0];
	if (cmd == NULL)
		return ;
	abs_cmd = ft_strdup(cmd);
	if (!abs_cmd)
		safe_exit(data, MALLOC_FAIL);
	if (access(cmd, F_OK) == 0)
	{
		if (is_directory(cmd) == 1)
		{
			print_error(3, "minishell: ", cmd, ": Is a directory");
			free(abs_cmd);
			safe_exit(data, 126);
		}
	}
	else
	{
		free(abs_cmd);
		abs_cmd = get_abs_cmd(data, cmd);
	}
	if (abs_cmd == NULL || ft_strncmp("..", cmd, 3) == 0)
	{
		print_error(3, "minishell: ", cmd, ": command not found");
		safe_exit(data, COMMAND_NOT_FOUND);
	}
	if (execve(abs_cmd, data->cmd_list, data->env) == -1)
	{
		print_error(4, "minishell: ", abs_cmd, ": ", strerror(errno));
		ft_free(&abs_cmd);
		safe_exit(data, COMMAND_NOT_FOUND);
	}
}

void	create_cmd_list(t_var *data, t_exec *exec)
{
	int		i;
	t_exec	*temp;

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
		data->cmd_list[i] = fix_content(data, temp->data, true);
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
	char	*path_cmd;

	path = get_paths(data);
	if (!path)
		return (cmd);
	i = -1;
	while (path[++i] != NULL)
	{
		path_cmd = ft_strjoin2(path[i], cmd, "/");
		if (!path_cmd)
		{
			free_array(&path);
			safe_exit(data, MALLOC_FAIL);
		}
		if (access(path_cmd, F_OK) == 0)
		{
			free_array(&path);
			return (path_cmd);
		}
		ft_free(&path_cmd);
	}
	free_array(&path);
	return (NULL);
}
