/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:15:09 by cseriildii        #+#    #+#             */
/*   Updated: 2024/10/28 13:16:05 by pvass            ###   ########.fr       */
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
	data->has_child = 0;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		malloc_failed(data);
	data->p_table = create_table();
	if (data->p_table == NULL)
		malloc_failed(data);
	init_env(data);
	handle_signal_std(0, NULL, data);
	setup_signal(SIGINT, SIG_STANDARD);
	setup_signal(SIGTERM, SIG_STANDARD);
	setup_signal(SIGUSR1, SIG_STANDARD);
	setup_signal(SIGQUIT, SIG_IGNORE);
	setup_signal(SIGPIPE, SIG_STANDARD);
}

void	get_promt(t_var *data)
{
	char	*home;
	char	*tmp;

	home = ft_getenv(data, "HOME", false);
	if (home != NULL && ft_strncmp(data->pwd, home, ft_strlen(home)) == 0)
	{
		tmp = ft_strjoin("~", data->pwd + ft_strlen(home));
		if (!tmp)
			malloc_failed(data);
		data->promt = ft_strjoin(tmp, "$ ");
		free(tmp);
	}
	else
		data->promt = ft_strjoin(data->pwd, "$ ");
	if (!data->promt)
		malloc_failed(data);
}
