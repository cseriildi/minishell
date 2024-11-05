/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:59 by icseri            #+#    #+#             */
/*   Updated: 2024/11/05 12:34:17 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*generate_random_filename(t_var *data)
{
	int		rand_fd;
	char	*here_doc_filename;
	int		i;
	t_token	*new;

	here_doc_filename = ft_calloc(1, 19);
	if (here_doc_filename == NULL)
		malloc_failed(data);
	rand_fd = safe_open("/dev/urandom", READ, data);
	if (rand_fd == -1 || read(rand_fd, here_doc_filename, 18) != 18)
		ft_memcpy(here_doc_filename, "temp_heredoc_file", 18);
	else
	{
		i = -1;
		while (++i < 18)
			here_doc_filename[i] = ft_abs(here_doc_filename[i]) % 25 + 'A';
	}
	safe_close(&rand_fd);
	new = create_new_token(here_doc_filename, 0);
	if (!new)
		return (ft_free(&here_doc_filename), malloc_failed(data), NULL);
	add_token_to_front(&data->here_doc_filename, new);
	return (here_doc_filename);
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
	char	*filename;

	len = ft_strlen(limiter);
	filename = generate_random_filename(data);
	fd_to_write = safe_open(filename, CREATE, data);
	ft_free(&filename);
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
