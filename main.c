/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/10/23 19:56:04 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signals	signals;

int	main(void)
{
	t_var	*data;
	bool	is_interactive;
	char	*line;
	
	is_interactive = isatty(STDIN_FILENO);
	//is_interactive = true; //just to avoid get_next_line leaks
	data = malloc(sizeof(t_var));
	if (!data)
		return (MALLOC_FAIL);
	init(data);
	while (true)
	{
		data->missing_quote = false;
		get_promt(data);
		signals.interactive = 1;
		if (is_interactive)
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
		signals.interactive = 0;
		if (!data->line)
		{
			if (isatty(STDIN_FILENO))
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
