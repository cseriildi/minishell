/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:22 by icseri            #+#    #+#             */
/*   Updated: 2024/09/17 17:51:53 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	execute(t_var *data)
{
	//dummy
	data->tree = malloc(sizeof(t_exec *));
	if (!data->tree)
		safe_exit(data, MALLOC_FAIL);
	*data->tree = malloc(sizeof(t_exec));
	(*data->tree)->data = "seq1";
	(*data->tree)->type = 1;
	(*data->tree)->down = malloc(sizeof(t_exec));
	(*data->tree)->next = NULL;
	(*data->tree)->down->data = "echo";
	(*data->tree)->down->down->data = "5";
	(*data->tree)->down->down->down->data = NULL;
	//end of dummy

	if ((*data->tree)->next == NULL)
		only_one_sequence(data, (*data->tree)->down);
	else
	{
		first_sequence(data, (*data->tree)->down);
		(*data->tree) = (*data->tree)->next;
		while ((*data->tree)->next != NULL)
		{
			middle_sequence(data, (*data->tree)->down);
			(*data->tree) = (*data->tree)->next;
		}
		if ((*data->tree)->next == NULL)
			last_sequence(data, (*data->tree)->down);
	}
}

void	only_one_sequence(t_var *data, t_exec *tree)
{
	if (is_builtin(tree->data) == true)
		exec_sequence(data, tree, STDIN_FILENO, STDOUT_FILENO);
	else
	{
		data->pid = fork();
		if (data->pid == -1)
			safe_exit(data, FORK_FAIL);
		if (data->pid == 0)
			exec_sequence(data, tree, STDIN_FILENO, STDOUT_FILENO);
		else
		{
			waitpid(data->pid, &data->exit_status, 0);
			if (WIFEXITED(data->exit_status))
				data->exit_code = WEXITSTATUS(data->exit_status);
			printf("This is just for debugging!!\nexit code: %d\n", data->exit_code);
		}
	}
}

void	first_sequence(t_var *data, t_exec *tree)
{
	pipe(data->pipe1_fd);
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		close(data->pipe1_fd[0]);
		exec_sequence(data, tree, STDIN_FILENO, data->pipe1_fd[1]);
		exit(EXIT_SUCCESS);
	}
}

void	middle_sequence(t_var *data, t_exec *tree)
{
	pipe(data->pipe2_fd);
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		close(data->pipe1_fd[1]);
		close(data->pipe2_fd[0]);
		exec_sequence(data, tree, data->pipe1_fd[0], data->pipe2_fd[1]);
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

void	last_sequence(t_var *data, t_exec *tree)
{
	data->pid = fork();
	if (data->pid == -1)
		safe_exit(data, FORK_FAIL);
	if (data->pid == 0)
	{
		close(data->pipe1_fd[1]);
		exec_sequence(data, tree, data->pipe1_fd[0], STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(data->pid, &data->exit_status, 0);
		if (WIFEXITED(data->exit_status))
			data->exit_code = WEXITSTATUS(data->exit_status);
		printf("This is just for debugging!!\nexit code: %d\n", data->exit_code);
	}
}

void	exec_sequence(t_var *data, t_exec *tree, int read, int write)
{
	char	**cmd_list;

	cmd_list = create_cmd_list(data, tree);
	while (tree != NULL && tree->type == WORD)
		tree = tree->down;
	while (tree != NULL)
	{
		redirect(data, tree);
		tree = tree->down;
	}
	dup_pipes(data, read, write);
	if (cmd_list != NULL)
	{
		if (exec_builtin(data, cmd_list) == false)
			exec_command(data, cmd_list);
	}
}

void	redirect(t_var *data, t_exec *tree)
{
	int	fd;

	if (tree->type == RED_IN)
	{
		fd = open(tree->data, O_RDONLY);
		if (fd == -1)
		{
			print_error(4, "minishell: ", tree->data, ": ", strerror(errno));
			data->exit_code = CANNOT_OPEN_FILE;
			return ;
		}
		if (dup2(fd, STDIN_FILENO) == -1)
			safe_exit(data, DUP2_FAIL);
	}
	else if (tree->type == RED_OUT)
	{
		fd = open(tree->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			print_error(4, "minishell: ", tree->data, ": ", strerror(errno));
			data->exit_code = CANNOT_OPEN_FILE;
			return ;
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			safe_exit(data, DUP2_FAIL);
	}
	else if (tree->type == APPEND)
	{
		fd = open(tree->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			print_error(4, "minishell: ", tree->data, ": ", strerror(errno));
			data->exit_code = CANNOT_OPEN_FILE;
			return ;
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			safe_exit(data, DUP2_FAIL);
	}
	else if (tree->type == HERE_DOC)
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
	if (execve(cmd_list[0], cmd_list, data->env) == -1)
	{
		print_error(2, COMMAND_NOT_FOUND, cmd_list[0]);
		exit(EXIT_FAILURE);
	}
}

char	**create_cmd_list(t_var *data, t_exec *tree)
{
	int		i;
	char	**cmd_list;

	//I have to extend the variables and remove the quotes here
	i = 0;
	while (tree != NULL && tree->type == WORD)
	{
		i++;
		tree = tree->down;
	}
	cmd_list = malloc(sizeof(char *) * (i + 1));
	if (!cmd_list)
		safe_exit(data, MALLOC_FAIL);
	i = 0;
	while (tree != NULL && tree->type == WORD)
	{
		cmd_list[i] = tree->data;
		i++;
		tree = tree->down;
	}
	cmd_list[i] = NULL;
	return (cmd_list);
}

bool	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) && ft_strncmp(cmd, "cd", 3) && ft_strncmp(cmd, "pwd", 4) && ft_strncmp(cmd, "export", 7) && ft_strncmp(cmd, "unset", 5) && ft_strncmp(cmd, "env", 4) && ft_strncmp(cmd, "exit", 5))
		return (false);
	return (true);
}