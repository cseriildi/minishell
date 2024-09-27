/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_table_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:33:56 by pvass             #+#    #+#             */
/*   Updated: 2024/09/27 13:12:08 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_table(t_table **p_table)
{
	t_table	*prev;
	t_table	*curr;

	if (p_table == NULL)
		return ;
	curr = *p_table;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		free(prev);
		prev = NULL;
	}
	*p_table = NULL;
}
