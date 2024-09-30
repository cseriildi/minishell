/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:01:43 by icseri            #+#    #+#             */
/*   Updated: 2024/09/30 11:14:13 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	get_next_token(t_var *data)
{
	if (data->line[data->index] == '|')
		pipes(data);
	else if (data->line[data->index] == '<')
		input(data);
	else if (data->line[data->index] == '>')
		redirection(data);
	else if (data->line[data->index])
		word(data);
}

void	lexer(t_var *data)
{
	t_token	*new_token;

	data->tokens = NULL;
	data->index = 0;
	while (data->line[data->index])
	{
		while (data->line[data->index] && data->line[data->index] == ' ')
			data->index++;
		if (data->line[data->index])
			get_next_token(data);
		if (data->missing_quote == true)
			return ;
	}
	new_token = create_new_token("END", END);
	if (new_token == NULL)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(&data->tokens, new_token);
}
