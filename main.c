/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/09/27 12:28:35 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signals signals;

void	init(t_var *data)
{
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
}

int	main(int argc, char **argv)
{
	t_var	*data;

	signals.child_pid = -1;
	signals.interactive = 1;
	(void)argv;
	if (argc != 1)
		return (print_error(2, "too many arguments, rerun like ./minishell"), ERROR_MISUSE);
	data = malloc(sizeof(t_var));
	if (!data)
		return (MALLOC_FAIL);
	init(data);
	init_signals();
	while (true)
	{
		get_promt(data);
		signals.interactive = 1;
		data->line = readline(data->promt);
		signals.interactive = 0;
		if (!data->line)
		{
			ft_printf("exit\n");
			safe_exit(data, READLINE_FAIL);
		}
		if (*data->line)
			add_history(data->line);
		lexer(data);
		parse(data);
		execute(data);
		free_all(data);
	}
	safe_exit(data, data->exit_code);
}
