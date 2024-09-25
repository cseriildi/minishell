/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:19:34 by pvass             #+#    #+#             */
/*   Updated: 2024/09/25 14:31:22 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_stack(t_stack **stack)
{
	t_stack *temp;
	t_stack *s;
	
	if (stack == NULL || *stack == NULL)
		return;
	s = *stack;
	while (s != NULL)
	{
		if (s->data != NULL)
			ft_free(&s->data);
		temp = s;
		s = s->next;
		free(temp);
	}
}

t_stack *copy_node(t_stack *node)
{
	t_stack *new;

	new = init_stack();
	if (new == NULL)
		return NULL;
	new->data = node->data;
	new->next = NULL;
	new->state = node->state;
	new->type = node->type;
	return (new);
}

void	stack_reverse(t_stack **stack)
{
	t_stack	*temp;
	t_stack	*temp2;
	t_stack *new;

	temp = *stack;
	new = NULL;
	while (temp != NULL)
	{
		temp2 = temp;
		temp = temp->next;
		stack_add_front(&new, temp2);
	}
	*stack = new;
}