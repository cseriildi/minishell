/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 11:49:01 by icseri            #+#    #+#             */
/*   Updated: 2024/07/19 16:03:18 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
