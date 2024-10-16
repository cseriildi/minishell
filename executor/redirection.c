/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:57:28 by icseri            #+#    #+#             */
/*   Updated: 2024/10/16 16:55:52 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	redirect_in(t_var *data, t_exec *exec, int read_fd)
{
	int		fd;
	t_exec	*temp;

	fd = -1;
	temp = exec;
	while (data->cmd_list[0] && temp != NULL)
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
	if (fd == -1)
		fd = read_fd;
	safe_dup2(&fd, STDIN_FILENO, data);
	safe_close(&fd);
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
	if (fd == -1)
		fd = write_fd;
	if (data->exec->next != NULL || (data->exec->next == NULL && data->cmd_list[0] && !is_builtin(data->cmd_list[0])))
		safe_dup2(&fd, STDOUT_FILENO, data);
	safe_close(&fd);
	return (true);
}
