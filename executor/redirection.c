/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:57:28 by icseri            #+#    #+#             */
/*   Updated: 2024/10/22 08:34:39 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	redirect_in(t_var *data, t_exec *exec, int read_fd)
{
	int		fd;
	t_exec	*temp;

	fd = -1;
	temp = exec;
	while (data->cmd_list && data->cmd_list[0] && temp != NULL)
	{
		if (temp->type == RED_IN)
		{
			safe_close(&fd);
			fd = safe_open(temp->data, READ, data);
		}
		else if (temp->type == HERE_DOC)
		{
			safe_close(&fd);
			fd = safe_open(data->here_doc_filename, READ, data);
		}
		else
		{
			temp = temp->down;
			continue ;
		}
		if (fd == -1)
			return (delete_file(data, data->here_doc_filename), false);
		temp = temp->down;
	}
	if (data->cmd_list && data->cmd_list[0])
	{
		if (fd == -1)
			safe_dup2(&read_fd, STDIN_FILENO, data);
		else
			safe_dup2(&fd, STDIN_FILENO, data);
	}
	safe_close(&fd);
	safe_close(&read_fd);
	delete_file(data, data->here_doc_filename);
	return (true);
}

bool	redirect_out(t_var *data, t_exec *exec, int write_fd)
{
	int		fd;
	t_exec	*temp;

	fd = -1;
	temp = exec;
	while (temp != NULL)
	{
		if (temp->type == RED_OUT)
		{
			safe_close(&fd);
			fd = safe_open(temp->data, CREATE, data);
		}
		else if (temp->type == APPEND)
		{
			safe_close(&fd);
			fd = safe_open(temp->data, CREATE_OR_APPEND, data);
		}
		else
		{
			temp = temp->down;
			continue ;
		}
		if (fd == -1)
			return (false);
		temp = temp->down;
	}
	if (data->cmd_list && data->cmd_list[0])
	{
		if (data->proc_count == 0)
		{
			if (fd != -1)
				data->fd_to_write = fd;
			return (true);
		}
		if (fd == -1)
			safe_dup2(&write_fd, STDOUT_FILENO, data);
		else
			safe_dup2(&fd, STDOUT_FILENO, data);
	}
	safe_close(&fd);
	safe_close(&write_fd);
	return true;
}
