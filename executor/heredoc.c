/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:59 by icseri            #+#    #+#             */
/*   Updated: 2024/10/22 11:30:58 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	generate_random_filename(t_var *data)
{
	int		rand_fd;
	
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

void print_heredoc(t_var *data, int fd)
{
	t_token	*current;

	current = data->heredoc_input;
	while (current != NULL)
	{
		ft_putstr_fd(current->content, fd);
		current = current->next;
	}
	data->is_heredoc = false;
	free_tokens(&data->heredoc_input);
}

void	do_heredoc(t_var *data, char *limiter, bool expanding)
{
	char	*line;
	int		fd_to_write;

	delete_file(data);
	data->here_doc_filename = ft_calloc(1, 19);
	if (data->here_doc_filename == NULL)
		safe_exit(data, MALLOC_FAIL);
	generate_random_filename(data);
	fd_to_write = safe_open(data->here_doc_filename, CREATE, data);
	if (fd_to_write == -1)
		return ;
	while (true)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (ft_strchr(line, '\n') == NULL)
		{
			ft_free(&line);
			print_error(3, "\nminishell: warning: here-document delimited by end-of-file (wanted `", limiter, "')");
			break;
		}
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] ==  '\n' )
		{
			ft_free(&line);
			break ;
		}
		if (expanding == true)
		{
			data->is_heredoc = true;
			if (expand(line, data, false) == MALLOC_FAIL)
			{
				safe_close(&fd_to_write);
				ft_free(&line);
				print_error(1, "minishell: malloc failed");	
				safe_exit(data, MALLOC_FAIL);
			}
			print_heredoc(data, fd_to_write);
		}
		else
		{
			ft_putstr_fd(line, fd_to_write);
			ft_free(&line);	
		}
	}
	safe_close(&fd_to_write);
}


void heredoc(t_var *data, t_exec *seq)
{
	t_exec *current;
	bool	expandable;

	current = seq;
	while (current)
	{
		if (current->type == HERE_DOC)
		{
			expandable = !ft_strchr(current->data, '\'') && !ft_strchr(current->data, '\"');
			fix_content(data, current, false);
			do_heredoc(data, current->data, expandable);
		}
		current = current->down;
	}
}