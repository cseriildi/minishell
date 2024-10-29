/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:59 by icseri            #+#    #+#             */
/*   Updated: 2024/10/29 22:03:44 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	generate_random_filename(t_var *data)
{
	int	rand_fd;
	int	i;

	delete_file(data);
	data->here_doc_filename = ft_calloc(1, 19);
	if (data->here_doc_filename == NULL)
		malloc_failed(data);
	rand_fd = safe_open("/dev/urandom", READ, data);
	if (rand_fd == -1)
	{
		ft_memcpy(data->here_doc_filename, "temp_heredoc_file", 18);
		return ;
	}
	if (read(rand_fd, data->here_doc_filename, 18) != 18)
	{
		ft_memcpy(data->here_doc_filename, "temp_heredoc_file", 18);
		safe_close(&rand_fd);
		return ;
	}
	i = -1;
	while (++i < 18)
		data->here_doc_filename[i] = (ft_abs(data->here_doc_filename[i])
				% 25) + 'A';
	safe_close(&rand_fd);
}

void	print_heredoc(t_var *data, int fd)
{
	t_token	*current;

	current = data->heredoc_input;
	while (current != NULL)
	{
		ft_putendl_fd(current->content, fd);
		current = current->next;
	}
	free_tokens(&data->heredoc_input);
}

void	write_to_temp_heredoc(t_var *data, char *line, bool expanding, int *fd)
{
	if (expanding == true)
	{
		if (expand(line, data, false) == MALLOC_FAIL)
		{
			safe_close(fd);
			ft_free(&line);
			malloc_failed(data);
		}
		print_heredoc(data, *fd);
	}
	else
	{
		ft_putstr_fd(line, *fd);
		ft_putstr_fd("\n", *fd);
		ft_free(&line);
	}
}

bool	do_heredoc(t_var *data, char *limiter, bool expanding)
{
	char	*line;
	int		fd_to_write;
	int		len;

	len = ft_strlen(limiter);
	generate_random_filename(data);
	fd_to_write = safe_open(data->here_doc_filename, CREATE, data);
	data->is_heredoc = true;
	setup_signal(SIGQUIT, SIG_STANDARD);
	while (fd_to_write != -1)
	{
		line = read_heredoc(limiter, len);
		if (line == NULL)
			break ;
		if (data->is_heredoc == FALSE)
			return (safe_close(&fd_to_write), ft_free(&line), false);
		write_to_temp_heredoc(data, line, expanding, &fd_to_write);
	}
	setup_signal(SIGQUIT, SIG_IGNORE);
	data->is_heredoc = false;
	safe_close(&fd_to_write);
	return (true);
}

bool	heredoc(t_var *data, t_exec *seq)
{
	t_exec	*current;
	bool	expandable;

	current = seq;
	while (current)
	{
		if (current->type == HERE_DOC)
		{
			expandable = !ft_strchr(current->data, '\'')
				&& !ft_strchr(current->data, '\"');
			fix_content(data, current, false);
			if (do_heredoc(data, current->data, expandable) == FALSE)
				return (FALSE);
		}
		current = current->down;
	}
	return (TRUE);
}
