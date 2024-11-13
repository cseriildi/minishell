/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/11/13 09:15:22 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig_num = 0;

int	main(void)
{
	t_var	*data;

	data = ft_calloc(sizeof(t_var), 1);
	if (!data)
		malloc_failed(data);
	init(data);
	while (true)
	{
		if (g_sig_num == SIGINT + 128)
			data->exit_code = 130;
		sig_hand(MAIN);
		read_input(data);
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
}
