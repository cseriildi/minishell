/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/10/01 11:42:28 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signals	signals;

int	main(void)
{
	t_var	*data;

	data = malloc(sizeof(t_var));
	if (!data)
		return (MALLOC_FAIL);
	init(data);
	//signals.exit_c = 0;
	while (true)
	{
		data->missing_quote = false;
		get_promt(data);
		signals.interactive = 1;
		data->line = readline(data->promt);
		//data->exit_code = signals.exit_c;
		signals.interactive = 0;
		if (!data->line)
		{
			ft_putendl_fd("exit", STDERR_FILENO);
			safe_exit(data, data->exit_code);
		}
		if (*data->line)
			add_history(data->line);
		lexer(data);
		if (data->missing_quote == true)
		{
			free_all(data);
			continue ;
		}
		parse(data);
		execute(data);
		free_all(data);
	}
	safe_exit(data, data->exit_code);
}
