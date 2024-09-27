/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:59 by icseri            #+#    #+#             */
/*   Updated: 2024/09/27 13:17:36 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*generate_random_filename(t_var *data)
{
	int		rand_fd;
	char	*filename;

	filename = "temp_heredoc_file";
	rand_fd = safe_open("/dev/urandom", 'R', data);
	if (rand_fd == -1)
		return (filename);
	if (read(rand_fd, filename, 10) != 10)
	{
		safe_close(&rand_fd);
		return (filename);
	}
	safe_close(&rand_fd);
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
		return (false);
	fd_to_read = safe_open(filename, READ, data);
	if (fd_to_read == -1)
	{
		safe_close(&fd_to_write);
		return (false);
	}
	while (true)
	{
		ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
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
				safe_close(&fd_to_write);
				safe_close(&fd_to_read);
				safe_exit(data, MALLOC_FAIL);
			}
			line = expanded_line;
		}
		ft_putstr_fd(line, fd_to_write);
		ft_free(&line);
	}
	safe_close(&fd_to_write);
	safe_dup2(&fd_to_read, STDIN_FILENO, data);
	delete_file(data, filename);
	return (true);
}
