/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/09/27 13:15:51 by cseriildii       ###   ########.fr       */
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
