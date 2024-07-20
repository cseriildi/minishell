/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:11:51 by pvass             #+#    #+#             */
/*   Updated: 2024/07/20 15:10:25 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

t_stack *init_stack()
{
	t_stack	*state_0;

	state_0 = (t_stack *)malloc(sizeof(*state_0));
	if (!state_0)
		return (NULL);
	state_0->data = NULL;
	state_0->state = 0;
	state_0->type = END;
	state_0->next = NULL;
	return (state_0);
}

t_stack	*new_stack_node(t_token *token)
{
	t_stack	*state_0;

	state_0 = (t_stack *)malloc(sizeof(*state_0));
	if (!state_0)
		return (NULL);
	state_0->data = token->content;
	state_0->state = 1;
	state_0->type = token->type;
	state_0->next = NULL;
	return (state_0);
}

void	stack_add_front(t_stack **stack, t_stack *new)
{
	if (stack == NULL)
		return ;
	new -> next = *stack;
	*stack = new;
}

void	del_stack_node(t_stack **stack)
{
	if (stack == NULL || *stack == NULL)
		return ;
	free((*stack)->next);
	free(*stack);
}

t_stack	*pop_stack(t_stack **stack)
{
	t_stack *new;

	if (stack == NULL || *stack == NULL)
		return NULL;
	new = *stack;
	*stack = (*stack)->next;
	new->next = NULL;
	return (new);
	//del_stack_node(&temp);
}

