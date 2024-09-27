/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 14:02:02 by icseri            #+#    #+#             */
/*   Updated: 2024/09/27 13:41:00 by cseriildii       ###   ########.fr       */
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
		safe_exit(data, MALLOC_FAIL);
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
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(&data->tokens, new_token);
}

void	pipes(t_var *data)
{
	t_token	*new_token;

	data->index++;
	new_token = create_new_token("|", PIPE);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(&data->tokens, new_token);
}
