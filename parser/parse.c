/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 16:03:07 by pvass             #+#    #+#             */
/*   Updated: 2024/09/25 13:21:28 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h" //cat < in | cat ???

t_table	*get_entry(t_token *token, t_table *p_table, t_stack *stack)
{
	int		input_type;
	t_table	*table_entry;

	if (!stack)
		return (NULL);
	table_entry = NULL;
	input_type = -1;
	if (token)
		input_type = token->type;
	while (p_table != NULL)
	{
		if (p_table->state == stack->state)
		{
			if (p_table->event == input_type)
				return (p_table);
			else if (p_table->event == P_NONE)
				table_entry = p_table;
		}
		p_table = p_table->next;
	}
	return (table_entry);
}

int	get_next_state(t_table *p_table, t_stack *stack)
{
	int	next_state;

	next_state = 0;
	while (p_table != NULL)
	{
		if (p_table->state == stack->next->state)
		{
			if (p_table->event == stack->type)
				return (p_table->next_s);
			else if (p_table->event == P_NONE)
				next_state = p_table->next_s;
		}
		p_table = p_table->next;
	}
	return (next_state);
}

void put_pipes_right_place(t_stack **result)
{
	t_stack *temp;
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
	if(temp->type == P_PIPE_SEQ && temp->next->type == PIPE)
	{
		*result = temp->next->next;
		temp2 = *result;
		swap_stack(&temp, &(temp->next));
		while (temp2->next != NULL)
		{
			if (temp2->next->type == 102 && temp2->type != 101)
			{
				temp->next->next = temp2->next;
				temp2->next = temp;
				break;
			}
			temp2 = temp2->next;
		}
	}	
}

void	parse(t_var *data)
{
	t_stack	*stack;
	t_table	*entry;
	t_stack	*result;
	t_token	*token_list;
	int		run;

	run = TRUE;
	stack = init_stack();
	if (stack == NULL)
		safe_exit(data, MALLOC_FAIL);
	result = NULL;
	if (stack == NULL)
		return ;
	token_list = data->tokens;
	while (run == 1)
	{
		entry = get_entry(token_list, data->p_table, stack);
		if (entry && entry->action == A_SHIFT)
			run = shift(&stack, &data->tokens, entry->next_s);
		else if (entry && entry->action == A_REDUCE)
			run = reduce(&stack, data->p_table, entry, &result);
		else if (entry && entry->action == A_ACCEPT)
		{
			run = 0;
			free_stack(&stack);
			put_pipes_right_place(&result);
			data->exec = create_exec(&result);
			free_stack(&result);
			if (data->exec == NULL)
				return ;
			reverse_exec(&data->exec);
			free_tokens(data);
		}
		else
		{
			run = 0;
			print_error(3, "minishell: syntax error near unexpected token '", token_list->content, "'");
			data->exit_code = CANNOT_OPEN_FILE;
			return (free_stack(&result), free_stack(&stack), free_tokens(data));
		}
		if (run == -1)
			return (free_stack(&result), free_stack(&stack), safe_exit(data, MALLOC_FAIL));
	}
}
