/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift_reduce.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:06:14 by pvass             #+#    #+#             */
/*   Updated: 2024/09/25 11:57:37 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_stack	*new_stack_node_state(int next_state)
{
	t_stack	*new;

	new = (t_stack *)malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->type = 1;
	new->state = next_state;
	new->data = NULL;
	new->next = NULL;
	return (new);
}

int	shift(t_stack **stack, t_token **token_list, int next_state)
{
	t_stack	*new;
	t_token *prev;

	new = new_stack_node(*token_list);
	if (new == NULL)
		return (-1);
	stack_add_front(stack, new);
	new = new_stack_node_state(next_state);
	if (new == NULL)
		return (-1);
	stack_add_front(stack, new);
	prev = *token_list;
	*token_list = (*token_list)->next;
	return (1);
}

int	push_reduction(t_stack **stack, int reduction)
{
	t_stack	*new;

	new = (t_stack *)malloc(sizeof(*new));
	if (!new)
		return (0);
	new->type = reduction;
	new->state = 0;
	new->data = NULL;
	new->next = *stack;
	*stack = new;
	return (1);
}

void	put_pipes_in_place(t_stack *stack, t_stack **result)
{
	int	i;
	t_stack	*temp;
	t_stack	*new;

	i = 0;
	new = NULL;
	while (i < 4)
	{
		temp = copy_node(stack);
		if (temp == NULL)
			return ;
		if (temp->type != P_NONE)
			stack_add_front(&new, temp);
		i++;
		stack = stack->next;
	}
	stack_reverse(&new);
	temp = new;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = *result;
	*result = new;
}

void	put_red_in_place(t_stack *stack, t_stack **result)
{
	int	i;
	t_stack	*temp;
	t_stack	*new;

	i = 0;
	new = NULL;
	while (i < 2)
	{
		temp = copy_node(stack);
		if (temp == NULL)
			return ;
		if (temp->type != P_NONE)
			stack_add_front(&new, temp);
		i++;
		stack = stack->next;
	}
	stack_reverse(&new);
	temp = new;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = *result;
	*result = new;
}

int	check_red(int type)
{
	int a;

	a = type;
	if (a == P_RED_IN || a == P_RED_OUT || a == P_APPEND || a == P_HERE_DOC)
		return (1);
	return (0);
}

int	reduce(t_stack **stack, t_table *p_table, t_table *entry, t_stack **result)
{
	t_stack	*popped;
	t_stack	*temp;
	int		i;
	int		next_state;

	i = 0;
	popped = NULL;
	while (i / 2 < entry->nb_reduce && (*stack)->next != NULL)
	{
		temp = pop_stack(stack);
		stack_add_front(&popped, temp);
		if (temp->type > P_NONE)
			stack_add_front(result, temp);
		else 
			free(temp);
		i++;
	}
	if (popped != NULL)
	{
		if (push_reduction(stack, entry->next_s))
		{
			next_state = get_next_state(p_table, *stack);
			temp = new_stack_node_state(next_state);
			if (temp == NULL)
				return (-1);
			stack_add_front(stack, temp);
		}
		else
			return (-1);
	}
	if (i / 2 == entry->nb_reduce)
		return (TRUE);
	else
		return (FALSE);
}
