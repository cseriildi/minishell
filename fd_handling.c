/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handling.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:50:15 by icseri            #+#    #+#             */
/*   Updated: 2024/10/16 14:01:58 by icseri           ###   ########.fr       */
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
		data->exit_code = CANNOT_OPEN_FILE;
		if (access(filename, X_OK) == -1)
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

void	delete_file(t_var *data, char *filename)
{
	if (filename == NULL || access(filename, F_OK) == -1)
		return ;
	if (unlink(filename) == -1)
	{
		print_error(4, "minishell: ", filename, ": ", strerror(errno));
		data->exit_code = UNLINK_FAIL;
	}
	//ft_free(&filename); //i don't know if it's allocated or not, have to check
	filename = NULL; //remove if it's allocated, otherwise remove the previous line
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
