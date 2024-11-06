/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handling.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:50:15 by icseri            #+#    #+#             */
/*   Updated: 2024/11/06 13:07:04 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	safe_open(char *filename, int mode, t_var *data)
{
	int	fd;

	if (mode == READ)
		fd = open(filename, O_RDONLY, 0644);
	else if (mode == CREATE)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == CREATE_OR_APPEND)
		fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		return (-1);
	if (fd == -1)
	{
		print_error(4, "minishell: ", filename, ": ", strerror(errno));
		data->exit_code = 1;
	}
	return (fd);
}

void	safe_close(int *fd)
{
	if (*fd < 3)
		return ;
	close(*fd);
	*fd = -1;
}

void	delete_file(t_var *data)
{
	t_token	*temp;

	temp = data->here_doc_filename;
	while (temp != NULL)
	{
		if (temp->content == NULL
			|| access(temp->content, F_OK) == -1)
		{
			temp = temp->next;
			continue ;
		}
		if (unlink(temp->content) == -1)
		{
			print_error(4, "minishell: ",
				temp->content, ": ", strerror(errno));
			data->exit_code = UNLINK_FAIL;
		}
		temp = temp->next;
	}
	free_tokens(&data->here_doc_filename);
}

void	safe_dup2(int *old_fd, int new_fd, t_var *data)
{
	if (*old_fd == new_fd || *old_fd < 0 || new_fd < 0)
		return ;
	if (dup2(*old_fd, new_fd) == -1)
	{
		print_error(2, strerror(errno), " :Cannot duplicate file descriptor");
		data->exit_code = DUP2_FAIL;
		safe_close(old_fd);
		safe_exit(data, DUP2_FAIL);
	}
	safe_close(old_fd);
}
