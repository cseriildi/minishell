/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift_reduce.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:06:14 by pvass             #+#    #+#             */
/*   Updated: 2024/09/23 13:40:13 by icseri           ###   ########.fr       */
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

	//printf("SHIFT\n");
	//printf("SHIFT1\n");
	//print_stack(*stack);
	new = new_stack_node(*token_list);
	if (new == NULL)
		return (printf("REJECT2\n"), 0);
	stack_add_front(stack, new);
	new = new_stack_node_state(next_state);
	if (new == NULL)
		return (printf("REJECT3\n"), 0);
	stack_add_front(stack, new);
	//printf("SHIFT2\n");
	//print_stack(*stack);
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

	/* printf("REDUCE\n"); */
	i = 0;
	popped = NULL;
	/* printf("I AM HERE\n");
	print_stack(*stack);
	printf("I AM HERE\n");
	print_stack(*result); */
	while (i / 2 < entry->nb_reduce && (*stack)->next != NULL)
	{
		temp = pop_stack(stack);
	//	printf("POP\n");
	//	print_stack(temp);
		stack_add_front(&popped, temp);
		if (temp->type > P_NONE)
			stack_add_front(result, temp);
		else 
		{
		//	printf("FREE\n");
			free(temp);
		}
		i++;
	}
	/* printf("I AM HERE2\n");
	print_stack(*result);
	printf("I AM HERE2\n");
	print_stack(*stack); */
	if (popped != NULL)
	{
	//	printf("I AM HERE\n");
		//print_stack(*stack);
		if (push_reduction(stack, entry->next_s))
		{
			next_state = get_next_state(p_table, *stack);
			temp = new_stack_node_state(next_state);
			if (temp == NULL)
				return (0);
			stack_add_front(stack, temp);
		}
		//print_stack(*stack);
	}
	//printf ("%d %d\n", i, entry->nb_reduce);
	if (i / 2 == entry->nb_reduce)
		return (TRUE);
	else
		return (FALSE);
}
