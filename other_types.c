/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 14:02:02 by icseri            #+#    #+#             */
/*   Updated: 2024/07/19 17:11:19 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection(t_var *data)
{
	t_token	*new_token;

	if (ft_strncmp(data->line + data->index, ">>", 2) == 0)
	{
		data->index += 2;
		new_token = create_new_token(">>", APPEND);
	}
	else
	{
		data->index++;
		new_token = create_new_token(">", RED_OUT);
	}
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
}

void	input(t_var *data)
{
	t_token	*new_token;

	if (ft_strncmp(data->line + data->index, "<<", 2) == 0)
	{
		data->index += 2;
		new_token = create_new_token("<<", HERE_DOC);
	}
	else
	{
		data->index++;
		new_token = create_new_token("<", RED_IN);
	}
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
}

void	pipes(t_var *data)
{
	t_token	*new_token;

	if (ft_strncmp(data->line + data->index, "||", 2) == 0)
	{
		data->index += 2;
		new_token = create_new_token("||", OR);
	}
	else
	{
		data->index++;
		new_token = create_new_token("|", PIPE);
	}
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
}

void	braces(t_var *data)
{
	t_token	*new_token;

	if (data->line[data->index] == '(')
		new_token = create_new_token("(", L_BRACKET);
	else
		new_token = create_new_token(")", R_BRACKET);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
	data->index++;
}

void	and_sign(t_var *data)
{
	t_token	*new_token;

	if (ft_strncmp(data->line + data->index, "&&", 2) != 0)
		safe_exit(data, ERROR_MISUSE);
	new_token = create_new_token("&&", AND);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
	data->index += 2;
}
