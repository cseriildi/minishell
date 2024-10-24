/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:22 by icseri            #+#    #+#             */
/*   Updated: 2024/10/24 21:31:35 by icseri           ###   ########.fr       */
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

bool	good_redir_path(char *redir, t_var *data)
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
	if (count == 0)
		return (1);
	new = ft_substr(redir, 0, count);
	if (new == NULL)
		malloc_failed(data);
	res = is_directory(new);
	free(new);
	return (res);
}

int	count_exec(t_exec *exec)
{
	t_exec	*temp;
	int		i;

	i = 0;
	temp = exec;
	while (temp != NULL)
	{
		i++;
		temp = temp->down;
	}
	return (i);
}

bool	redirs_exist(t_var *data, t_exec *exec)
{
	t_exec	*temp2;
	char	*filename;
	int		count;

	temp2 = exec;
	while (temp2 != NULL)
	{
		if (is_in_out_app(temp2) == 1)
		{
			count = count_exec(temp2);
			filename = ft_strdup(temp2->data);
			fix_content(data, temp2, true);
			if (!temp2->data || !*temp2->data || count != count_exec(temp2))
			{
				data->exit_code = 1;
				print_error(3, "minishell: ", filename, ": ambiguous redirect");
				free(filename);
				return (0);
			}
			free(filename);
			if ((temp2->type != RED_IN || exec->type == 2) && is_directory(temp2->data) == 1)
			{
				data->exit_code = 1;
				print_error(3, "minishell: ", temp2->data, ": Is a directory");
				return (0);
			}
			if (temp2->type != RED_IN && good_redir_path(temp2->data, data) == 0)
			{
				data->exit_code = 1;
				print_error(3, "minishell: ", temp2->data, ": No such file or directory");
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
	t_exec *temp;

	temp = exec;
	heredoc(data, temp);
	create_cmd_list(data, temp);
	if (!data->cmd_list || !*data->cmd_list || is_builtin(data->cmd_list[0]) == true)
		exec_sequence(data, temp, STDIN_FILENO, STDOUT_FILENO);
	else
	{
		data->proc_count++;
		data->pid = fork();
		if (data->pid == -1)
			safe_exit(data, FORK_FAIL);
		g_signals.child_pid = data->pid;
		if (data->pid == 0)
		{
			exec_sequence(data, temp, STDIN_FILENO, STDOUT_FILENO);
			safe_exit(data, data->exit_code);
		}
		else
		{
			waitpid(data->pid, &data->exit_status, 0);
			g_signals.child_pid = -1;
			if (WIFEXITED(data->exit_status))
				data->exit_code = WEXITSTATUS(data->exit_status);
			data->proc_count--;
		}
	}
}

void	first_sequence(t_var *data, t_exec *exec)
{
	t_exec *temp;

	temp = exec;
	heredoc(data, temp);
	create_cmd_list(data, temp);
	pipe(data->pipe1_fd);
	data->proc_count++;
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		safe_close(&data->pipe1_fd[0]);
		exec_sequence(data, temp, STDIN_FILENO, data->pipe1_fd[1]);
		safe_exit(data, data->exit_code);
	}
}

void	middle_sequence(t_var *data, t_exec *exec)
{
	t_exec *temp;

	temp = exec;
	heredoc(data, temp);
	create_cmd_list(data, temp);
	pipe(data->pipe2_fd);
	data->proc_count++;
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		safe_close(&data->pipe1_fd[1]);
		safe_close(&data->pipe2_fd[0]);
		exec_sequence(data, temp, data->pipe1_fd[0], data->pipe2_fd[1]);
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
	t_exec *temp;

	temp = exec;
	heredoc(data, temp);
	create_cmd_list(data, temp);
	data->proc_count++;
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		safe_close(&data->pipe1_fd[1]);
		exec_sequence(data, temp, data->pipe1_fd[0], STDOUT_FILENO);
		safe_exit(data, data->exit_code);
	}
	else
	{
		safe_close(&data->pipe1_fd[0]);
		safe_close(&data->pipe1_fd[1]);
		waitpid(data->pid, &data->exit_status, 0);
		if (WIFEXITED(data->exit_status))
			data->exit_code = WEXITSTATUS(data->exit_status);
		data->proc_count--;
		while (data->proc_count-- > 0)
			wait(NULL);
	}
}

void	exec_sequence(t_var *data, t_exec *exec, int read_fd, int write_fd)
{
	if (redirs_exist(data, exec) == 0)
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

int is_directory(const char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) != 0) {
        return 0;
    }
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
		malloc_failed(data);
	if(ft_strchr(cmd, '/') || *ft_getenv(data, "PATH", true) == '\0')
	{
		if (access(cmd, F_OK) != 0)
		{
			print_error(3, "minishell: ", cmd, ": No such file or directory");
			ft_free(&abs_cmd);
			safe_exit(data, COMMAND_NOT_FOUND);
		}
	}
	else
	{
		free(abs_cmd);
		abs_cmd = get_abs_cmd(data, cmd);
	}
	if (abs_cmd == NULL || ft_strncmp("..", cmd, 3) == 0 || ft_strncmp(".", cmd, 2) == 0)
	{
		print_error(2, cmd, ": command not found");
		ft_free(&abs_cmd);
		safe_exit(data, COMMAND_NOT_FOUND);
	}
	if (execve(abs_cmd, data->cmd_list, data->env) == -1)
	{
		if (ft_strncmp(cmd, abs_cmd, ft_strlen(cmd) + 1)  == 0 && is_directory(cmd) == 1)
		{
			print_error(2, cmd, ": Is a directory");
			ft_free(&abs_cmd);
			safe_exit(data, ERRORS_AFTER_EXECVE);
		}
		print_error(4, "minishell: ", abs_cmd, ": ", strerror(errno));
		ft_free(&abs_cmd);
		safe_exit(data, ERRORS_AFTER_EXECVE);
	}
}

void	create_cmd_list(t_var *data, t_exec *exec)
{
	int		i;
	t_exec	*temp;
	t_exec	*next;
	char *cmd;
	
	free_array(&data->cmd_list);
	temp = exec;
	if (temp->type != WORD)
		return;
	i = 0;
	while (temp != NULL && temp->type == WORD)
	{
		next = temp->down;
		fix_content(data, temp, true);
		temp = next;
	}
	temp = exec;
	while (temp != NULL)
	{
		if (temp->type == WORD)
			i++;
		temp = temp->down;
	}
	data->cmd_list = malloc(sizeof(char *) * (i + 1));
	if (!data->cmd_list)
		malloc_failed(data);
	temp = exec;
	i = 0;
	while (temp != NULL)
	{
		if (temp->type == WORD) {
			cmd = ft_strdup(temp->data);
			if (!cmd)
				malloc_failed(data);
			if (*cmd == '$' && *temp->data == '\0')
			{
				free(cmd);
				temp = temp->down;
				continue ;
			}
			data->cmd_list[i] = cmd;
			i++;
		}
		temp = temp->down;
	}
	data->cmd_list[i] = NULL;
}

char	**get_paths(t_var *data)
{
	int		i;
	char	**path;

	if (*ft_getenv(data, "PATH", true) == '\0')
		return (NULL);
	i = 0;
	while (data->env[i] != NULL)
	{
		if (ft_strncmp(data->env[i], "PATH=", 5) == 0)
		{
			path = ft_split(data->env[i] + 5, ':');
			if (!path)
				malloc_failed(data);
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

	if (!*cmd)
		return (NULL);
	if (*cmd == '/')
	{
		path_cmd = ft_strdup(cmd);
		if (path_cmd == NULL)
			malloc_failed(data);
		return (path_cmd);
	}
	if (*cmd == '.')
	{
		if (cmd[1] == '.' || cmd[1] == '/' || cmd[1] == '\0')
		{
			path_cmd = ft_strdup(cmd);
			if (path_cmd == NULL)
				malloc_failed(data);
			return (path_cmd);
		}
	}
	path = get_paths(data);
	if (!path)
	{
		path_cmd = ft_strdup(cmd);
		if (path_cmd == NULL)
			malloc_failed(data);
		return (path_cmd);
	}
	i = -1;
	while (path[++i] != NULL)
	{
		path_cmd = ft_strjoin2(path[i], cmd, "/");
		if (!path_cmd)
		{
			free_array(&path);
			malloc_failed(data);
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
