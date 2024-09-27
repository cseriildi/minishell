/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:15:09 by cseriildii        #+#    #+#             */
/*   Updated: 2024/09/27 13:28:34 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_var *data)
{
	signals.child_pid = -1;
	signals.interactive = 1;
	data->tokens = NULL;
	data->p_table = NULL;
	data->exec = NULL;
	data->env = NULL;
	data->promt = NULL;
	data->cmd_list = NULL;
	data->line = NULL;
	data->exit_code = 0;
	data->subshell_level = 1;
	data->pwd = NULL;
	data->pipe1_fd[0] = -1;
	data->pipe1_fd[1] = -1;
	data->pipe2_fd[0] = -1;
	data->pipe2_fd[1] = -1;
	data->stdout_copy = -1;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		safe_exit(data, MALLOC_FAIL);
	data->p_table = create_table();
	if (data->p_table == NULL)
		safe_exit(data, MALLOC_FAIL);
	init_env(data);
	init_signals();
}

void	get_promt(t_var *data)
{
	char	*home;
	char	*tmp;

	home = ft_getenv(data, "HOME");
	if (home != NULL && ft_strncmp(data->pwd, home, ft_strlen(home)) == 0)
	{
		tmp = ft_strjoin("~", data->pwd + ft_strlen(home));
		if (!tmp)
			safe_exit(data, MALLOC_FAIL);
		data->promt = ft_strjoin(tmp, "$ ");
		free(tmp);
	}
	else
		data->promt = ft_strjoin(data->pwd, "$ ");
	if (!data->promt)
		safe_exit(data, MALLOC_FAIL);
}