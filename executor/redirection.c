/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:57:28 by icseri            #+#    #+#             */
/*   Updated: 2024/10/24 21:10:19 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	open_redins(t_var *data, t_exec *exec, int *fd)
{
	t_exec	*temp;

	temp = exec;
	while (data->cmd_list && data->cmd_list[0] && temp != NULL)
	{
		if (temp->type == RED_IN)
		{
			safe_close(fd);
			*fd = safe_open(temp->data, READ, data);
		}
		else if (temp->type == HERE_DOC)
		{
			safe_close(fd);
			*fd = safe_open(data->here_doc_filename, READ, data);
		}
		else
		{
			temp = temp->down;
			continue ;
		}
		if (*fd == -1)
			return (false);
		temp = temp->down;
	}
	return (true);
}

bool	redirect_in(t_var *data, t_exec *exec, int *read_fd)
{
	int	fd;

	fd = -1;
	if (open_redins(data, exec, &fd) == false)
		return (delete_file(data), false);
	if (data->cmd_list && data->cmd_list[0] && data->proc_count > 0)
	{
		if (fd == -1)
			safe_dup2(read_fd, STDIN_FILENO, data);
		else
			safe_dup2(&fd, STDIN_FILENO, data);
	}
	safe_close(&fd);
	safe_close(read_fd);
	return (true);
}

bool	open_redouts(t_var *data, t_exec *exec, int *fd)
{
	t_exec	*temp;

	temp = exec;
	while (temp != NULL)
	{
		if (temp->type == RED_OUT)
		{
			safe_close(fd);
			*fd = safe_open(temp->data, CREATE, data);
		}
		else if (temp->type == APPEND)
		{
			safe_close(fd);
			*fd = safe_open(temp->data, CREATE_OR_APPEND, data);
		}
		else
		{
			temp = temp->down;
			continue ;
		}
		if (*fd == -1)
			return (false);
		temp = temp->down;
	}
	return (true);
}

bool	redirect_out(t_var *data, t_exec *exec, int *write_fd)
{
	int	fd;

	fd = -1;
	if (open_redouts(data, exec, &fd) == false)
		return (false);
	if (data->cmd_list && data->cmd_list[0])
	{
		if (data->proc_count == 0)
		{
			if (fd != -1)
				data->fd_to_write = fd;
			return (true);
		}
		if (fd == -1)
			safe_dup2(write_fd, STDOUT_FILENO, data);
		else
			safe_dup2(&fd, STDOUT_FILENO, data);
	}
	safe_close(&fd);
	safe_close(write_fd);
	return (true);
}
