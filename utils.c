/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:50:26 by icseri            #+#    #+#             */
/*   Updated: 2024/09/19 12:37:32 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return ;
	if (*tokens)
	{
		current = *tokens;
		while (current != NULL)
		{
			next = current->next;
			ft_free(&current->content);
			free(current);
			current = next;
		}
	}
	free(tokens);
}

void	safe_exit(t_var *data, int exit_code)
{
	if (data)
	{
		if (data->line)
			ft_free(&data->line);
		if (data->tokens)
			free_tokens(data->tokens);
		free(data);
	}
	exit(exit_code);
}

void	check_brackets(t_var *data)
{
	t_token	*head;
	t_token	*current;
	int		count;

	count = 0;
	head = *data->tokens;
	current = head;
	while (current != NULL)
	{
		if (current->type == L_BRACKET)
			count++;
		if (current->type == R_BRACKET)
			count--;
		if (count < 0)
			safe_exit(data, ERROR_MISUSE);
		current = current->next;
	}
	if (count != 0)
		safe_exit(data, ERROR_MISUSE);
}
