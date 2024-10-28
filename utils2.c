/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:55:59 by cseriildii        #+#    #+#             */
/*   Updated: 2024/10/28 11:38:17 by pvass            ###   ########.fr       */
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

	get_promt(data);
	if (isatty(STDIN_FILENO))
		data->line = readline(data->promt);
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
