/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:19:34 by pvass             #+#    #+#             */
/*   Updated: 2024/07/20 17:33:43 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_stack(t_stack **stack)
{
	t_stack *temp;
	t_stack *s;
	
	if (stack == NULL)
		return ;
	s = *stack;
	while (s != NULL)
	{
		free(s->data);
		temp = s;
		s = s->next;
		free(temp);
	}
}
