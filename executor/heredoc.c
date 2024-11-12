/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:59 by icseri            #+#    #+#             */
/*   Updated: 2024/11/12 20:44:51 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	generate_random_filename(t_var *data, t_exec *exec)
{
	int		rand_fd;
	int		i;

	ft_free(&exec->data);
	exec->data = ft_calloc(1, 24);
	if (exec->data == NULL)
		malloc_failed(data);
	rand_fd = safe_open("/dev/urandom", READ, data);
	ft_memcpy(exec->data, "/tmp/", 5);
	if (rand_fd == -1 || read(rand_fd, exec->data + 5, 18) != 18)
		ft_memcpy(exec->data + 5, "temp_heredoc_file", 18);
	else
	{
		i = 4;
		while (++i < 24)
			exec->data[i] = ft_abs(exec->data[i]) % 25 + 'A';
	}
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

bool	do_heredoc(t_var *data, t_exec *exec, bool expanding)
{
	char	*line;
	int		fd_to_write;
	int		len;

	data->limiter = ft_strdup(exec->data);
	if (!data->limiter)
		malloc_failed(data);
	len = ft_strlen(data->limiter);
	generate_random_filename(data, exec);
	fd_to_write = safe_open(exec->data, CREATE, data);
	data->is_heredoc = true;
	sig_hand(HEREDOC);
	while (fd_to_write != -1)
	{
		line = read_heredoc(data->limiter, len);
		if (line == NULL)
			break ;
		if (g_sig_num == SIGINT)
		{
			ft_free(&line);
			data->exit_code = 130;
			sig_hand(MAIN);
			data->is_heredoc = false;
			safe_close(&fd_to_write);
			return (false);
		}
		write_to_temp_heredoc(data, line, expanding, &fd_to_write);
	}
	sig_hand(MAIN);
	data->is_heredoc = false;
	safe_close(&fd_to_write);
	return (true);
}

bool	heredoc(t_var *data, t_exec *seq)
{
	t_exec	*current;
	t_exec	*top;
	bool	expandable;

	top = seq;
	while (top)
	{
		current = top;
		while (current)
		{
			if (current->type == HERE_DOC)
			{
				expandable = !ft_strchr(current->data, '\'')
					&& !ft_strchr(current->data, '\"');
				fix_content(data, current, false);
				if (do_heredoc(data, current, expandable) == FALSE)
					return (FALSE);
			}
			current = current->down;
		}
		top = top->next;
	}
	return (TRUE);
}
