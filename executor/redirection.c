/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:57:28 by icseri            #+#    #+#             */
/*   Updated: 2024/09/26 13:48:01 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	redirect_in(t_var *data, t_exec *exec)
{
	int		fd;
	t_exec	*temp;
	
	temp = exec;
	while (temp != NULL)
	{
		if (temp->type == RED_IN)
		{
			fd = safe_open(temp->data, READ, data);
			if (fd == -1)
				return false;
			safe_dup2(fd, STDIN_FILENO, data);
		}
		else if (temp->type == HERE_DOC)
		{
			if (here_doc(data, temp->data, false) == false)
				return false;
			//Do NOT forget about quotes in the LIMITER
		}
		temp = temp->down;
	}
	return true;
}
bool	redirect_out(t_var *data, t_exec *exec)
{
	int	fd;
	t_exec	*temp;

	temp = exec;
	while (temp != NULL)
	{
		if (exec->type == RED_OUT)
		{
			fd = safe_open(exec->data, CREATE, data);
			if (fd == -1)
				return false;
			safe_dup2(fd, STDOUT_FILENO, data);
		}
		else if (exec->type == APPEND)
		{
			fd = safe_open(exec->data, CREATE_OR_APPEND, data);
			if (fd == -1)
				return false;
			safe_dup2(fd, STDOUT_FILENO, data);
		}
		temp = temp->down;
	}
	return true;
}