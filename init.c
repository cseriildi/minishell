/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:15:09 by cseriildii        #+#    #+#             */
/*   Updated: 2024/11/13 09:12:21 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_var *data)
{
	data->pid = -1;
	data->is_heredoc = false;
	data->missing_quote = false;
	data->pipe1_fd[0] = -1;
	data->pipe1_fd[1] = -1;
	data->pipe2_fd[0] = -1;
	data->pipe2_fd[1] = -1;
	data->fd_to_write = STDOUT_FILENO;
	data->to_join = 1;
	data->p_table = create_table();
	if (data->p_table == NULL)
		malloc_failed(data);
	init_env(data);
}

void	get_prompt(t_var *data)
{
	char	*home;
	char	*tmp;

	home = ft_getenv(data, "HOME", false);
	if (!data->pwd)
		data->pwd = safe_getcwd(data);
	if (!data->pwd)
		data->prompt = ft_strdup(".$ ");
	else if (home != NULL && ft_strncmp(data->pwd, home, ft_strlen(home)) == 0)
	{
		tmp = ft_strjoin("~", data->pwd + ft_strlen(home));
		if (!tmp)
			malloc_failed(data);
		data->prompt = ft_strjoin(tmp, "$ ");
		free(tmp);
	}
	else
		data->prompt = ft_strjoin(data->pwd, "$ ");
	if (!data->prompt)
		malloc_failed(data);
}
