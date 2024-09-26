/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:59 by icseri            #+#    #+#             */
/*   Updated: 2024/09/26 14:38:45 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*generate_random_filename(t_var *data)
{
	int		rand_fd;
	char	*filename;

	filename = "0123456789";
	rand_fd = safe_open("/dev/urandom", 'R', data);
	if (rand_fd == -1)
		return (filename);
	if (read(rand_fd, filename, 10) != 10)
	{
		safe_close(rand_fd, data);
		return (filename);
	}
	safe_close(rand_fd, data);
	return (filename);
}

bool	here_doc(t_var *data, char *limiter, bool expanding)
{
	char	*line;
	int		fd_to_write;
	int		fd_to_read;
	char	*filename;
	char	*expanded_line;

	filename = generate_random_filename(data);
	fd_to_write = safe_open(filename, CREATE, data);
	if (fd_to_write == -1)
		return false;
	fd_to_read = safe_open(filename, READ, data);
	if (fd_to_read == -1)
	{
		safe_close(fd_to_write, data);
		return false;
	}
	printf("LIMITER {%s}\n", limiter);
	while (true)
	{
		ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		printf("LINE {%s}\n", line);
		if (ft_strncmp(line, limiter, ft_strlen(limiter) /* + 1 */) == 0)
		{
			ft_free(&line);
			break ;
		}
		if (expanding == true)
		{
			expanded_line = expander(data, line);
			ft_free(&line);
			if (expanded_line == NULL)
			{
				safe_close(fd_to_write, data);
				safe_close(fd_to_read, data);
				safe_exit(data, MALLOC_FAIL);
			}
			line = expanded_line;
		}
		ft_putstr_fd(line, fd_to_write);
		ft_free(&line);
	}
	safe_close(fd_to_write, data);
	safe_dup2(fd_to_read, STDIN_FILENO, data);
	delete_file(data, filename);
	return true;
}
