/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:55:59 by cseriildii        #+#    #+#             */
/*   Updated: 2024/10/29 21:57:37 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin2(char *str1, char *str2, char *delimiter)
{
	char	*tmp;
	char	*new_str;

	tmp = ft_strjoin(str1, delimiter);
	if (!tmp)
		return (NULL);
	new_str = ft_strjoin(tmp, str2);
	free(tmp);
	if (!new_str)
		return (NULL);
	return (new_str);
}

void	malloc_failed(t_var *data)
{
	print_error(1, "minishell: malloc failed");
	safe_exit(data, MALLOC_FAIL);
}

void	read_input(t_var *data)
{
	char	*line;

	get_prompt(data);
	if (isatty(STDIN_FILENO))
		data->line = readline(data->prompt);
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			data->line = ft_strtrim(line, "\n");
			free(line);
		}
	}
	if (!data->line)
	{
		if (isatty(STDIN_FILENO))
			ft_putendl_fd("exit", STDERR_FILENO);
		safe_exit(data, data->exit_code);
	}
	if (*data->line)
		add_history(data->line);
}

int	ft_abs(char c)
{
	if (c < 0)
		return (c * -1);
	else
		return (c);
}

char	*read_heredoc(char *limiter, int len)
{
	char	*line;
	char	*temp;

	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			temp = ft_strtrim(line, "\n");
			ft_free(&line);
			line = temp;
		}
	}
	if (!line || ft_strncmp(line, limiter, len) == 0)
	{
		if (!line)
			print_error(4, "minishell: warning: here-document ",
				"delimited by end-of-file (wanted `", limiter, "')");
		ft_free(&line);
		return (NULL);
	}
	return (line);
}
