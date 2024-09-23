/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:01:43 by icseri            #+#    #+#             */
/*   Updated: 2024/09/23 12:48:34 by icseri           ###   ########.fr       */
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
	data->tokens = malloc(sizeof(t_token *));
	if (!data->tokens)
		safe_exit(data, MALLOC_FAIL);
	data->tokens = NULL;
	data->index = 0;
	while (data->line[data->index])
	{
		while (data->line[data->index] && data->line[data->index] == ' ')
			data->index++;
		if (data->line[data->index])
			get_next_token(data);
	}
	add_token_to_back(
		&data->tokens, create_new_token("END", END));
}
