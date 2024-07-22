/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:50:26 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 15:20:27 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_var *data)
{
	t_token	*current;
	t_token	*next;

	if (!data->tokens)
		return ;
	if (*data->tokens)
	{
		current = *data->tokens;
		while (current != NULL)
		{
			next = current->next;
			ft_free(&current->content);
			free(current);
			current = next;
		}
	}
	free(data->tokens);
	data->tokens = NULL;
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr && arr[i])
			ft_free(&arr[i++]);
		free(arr);
	}
}

void	safe_exit(t_var *data, int exit_code)
{
	if (data)
	{
		if (data->line)
			ft_free(&data->line);
		if (data->tokens)
			free_tokens(data);
		ft_free(&data->pwd);
		ft_free(&data->promt);
		ft_free(&data->line);
		free_env(data->env);
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

void	get_promt(t_var *data)
{
	data->promt = ft_strjoin(data->pwd, " > ");
	if (!data->promt)
		safe_exit(data, MALLOC_FAIL);
}
