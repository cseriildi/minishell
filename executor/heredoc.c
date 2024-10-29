/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:59 by icseri            #+#    #+#             */
/*   Updated: 2024/10/29 16:00:19 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	generate_random_filename(t_var *data)
{
	int		rand_fd;

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
	safe_close(&rand_fd);
}

void	print_heredoc(t_var *data, int fd)
{
	t_token	*current;

	current = data->heredoc_input;
	while (current != NULL)
	{
		ft_putstr_fd(current->content, fd);
		current = current->next;
	}
	//data->is_heredoc = false;
	free_tokens(&data->heredoc_input);
}

void	write_to_temp_heredoc(t_var *data, char *line, bool expanding, int *fd)
{
	if (expanding == true)
	{
		//data->is_heredoc = true;
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
	while (fd_to_write != -1)
	{
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (data->is_heredoc == FALSE)
			return (safe_close(&fd_to_write), ft_free(&line), false);
		if (ft_strchr(line, '\n') == NULL
			|| (ft_strncmp(line, limiter, len) == 0
				&& line[ft_strlen(limiter)] == '\n'))
		{
			if (ft_strchr(line, '\n') == NULL)
				print_error(4, "minishell: warning: here-document ",
					"delimited by end-of-file (wanted `", limiter, "')");
			ft_free(&line);
			break ;
		}
		write_to_temp_heredoc(data, line, expanding, &fd_to_write);
	}
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
