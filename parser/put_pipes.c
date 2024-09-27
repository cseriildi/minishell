/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:07 by pvass             #+#    #+#             */
/*   Updated: 2024/09/27 12:42:13 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	put_pipes_right_place_util(t_stack **temp_a, t_stack **temp2_a)
{
	t_stack	*temp;
	t_stack	*temp2;

	temp = *temp_a;
	temp2 = *temp2_a;
	while (temp2->next != NULL)
	{
		if (temp2->next->type == 102 && temp2->type != 101)
		{
			temp->next->next = temp2->next;
			temp2->next = temp;
			break ;
		}
		temp2 = temp2->next;
	}
}

void	put_pipes_right_place(t_stack **result)
{
	t_stack	*temp;
	t_stack	*temp2;

	temp = *result;
	if (*result == NULL)
		return ;
	while (temp != NULL)
	{
		if (temp->type == P_PIPE_SEQ && temp->next->type == PIPE)
			swap_stack(&temp, &(temp->next));
		temp = temp->next;
	}
	temp = *result;
	if (temp->type == P_PIPE_SEQ && temp->next->type == PIPE)
	{
		*result = temp->next->next;
		temp2 = *result;
		swap_stack(&temp, &(temp->next));
		put_pipes_right_place_util(&temp, &temp2);
	}
}
