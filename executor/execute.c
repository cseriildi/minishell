/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:22 by icseri            #+#    #+#             */
/*   Updated: 2024/10/25 12:07:09 by pvass            ###   ########.fr       */
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

void	exec_command_2(t_var *data, char *abs_cmd, char *cmd)
{
	if (abs_cmd == NULL || ft_strncmp("..", cmd, 3) == 0
		|| ft_strncmp(".", cmd, 2) == 0)
	{
		print_error(2, cmd, ": command not found");
		ft_free(&abs_cmd);
		safe_exit(data, COMMAND_NOT_FOUND);
	}
	if (execve(abs_cmd, data->cmd_list, data->env) == -1)
	{
		if (ft_strncmp(cmd, abs_cmd, ft_strlen(cmd) + 1) == 0
			&& is_directory(cmd) == 1)
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
	if (ft_strchr(cmd, '/') || *ft_getenv(data, "PATH", true) == '\0')
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
	exec_command_2(data, abs_cmd, cmd);
}
