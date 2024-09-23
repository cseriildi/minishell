/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:19:34 by pvass             #+#    #+#             */
/*   Updated: 2024/09/19 14:36:20 by pvass            ###   ########.fr       */
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
			free(s->data);
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
		//ft_printf("%p\n", temp2);
		//print_stack(new);
		stack_add_front(&new, temp2);
		//print_stack(new);
	}
	*stack = new;
}