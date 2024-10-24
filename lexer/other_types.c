/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 14:02:02 by icseri            #+#    #+#             */
/*   Updated: 2024/10/24 12:56:38 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	redirection(t_var *data)
{
	t_token	*new_token;

	data->index++;
	if (data->line[data->index] == '>')
	{
		data->index++;
		new_token = create_new_token(">>", APPEND);
	}
	else
		new_token = create_new_token(">", RED_OUT);
	if (!new_token)
		malloc_failed(data);
	add_token_to_back(&data->tokens, new_token);
}

void	input(t_var *data)
{
	t_token	*new_token;

	data->index++;
	if (data->line[data->index] == '<')
	{
		data->index++;
		new_token = create_new_token("<<", HERE_DOC);
	}
	else
		new_token = create_new_token("<", RED_IN);
	if (!new_token)
		malloc_failed(data);
	add_token_to_back(&data->tokens, new_token);
}

void	pipes(t_var *data)
{
	t_token	*new_token;

	data->index++;
	new_token = create_new_token("|", PIPE);
	if (!new_token)
		malloc_failed(data);
	add_token_to_back(&data->tokens, new_token);
}
