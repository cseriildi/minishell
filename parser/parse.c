/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 16:03:07 by pvass             #+#    #+#             */
/*   Updated: 2024/10/01 15:46:42 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

int	parse_accept(t_var *data, t_stack *stack, t_stack *result)
{
	free_stack(&stack);
	put_pipes_right_place(&result);
	data->exec = create_exec(&result);
	if (data->exec == NULL && result != NULL)
		return (free_stack(&result), -1);
	free_stack(&result);
	reverse_exec(&data->exec);
	free_tokens(data);
	return (0);
}

int	parse_reject(t_var *data, t_stack *stack, t_stack *result)
{
	data->exit_code = 2;
	if (data->tokens->type != 0)
		print_error(3, "minishell: syntax error near unexpected token `",
		data->tokens->content, "'");
	if (data->tokens->type == 0)
		print_error(3, "minishell: syntax error near unexpected token `",
		"newline", "'");
	free_stack(&result);
	free_stack(&stack);
	free_tokens(data);
	return (0);
}

void	parse(t_var *data)
{
	t_stack	*stack;
	t_table	*entry;
	t_stack	*result;
	int		run;

	run = TRUE;
	stack = init_stack();
	if (stack == NULL)
		safe_exit(data, MALLOC_FAIL);
	result = NULL;
	while (run == 1)
	{
		entry = get_entry(data->tokens, data->p_table, stack);
		if (entry && entry->action == A_SHIFT)
			run = shift(&stack, &data->tokens, entry->next_s);
		else if (entry && entry->action == A_REDUCE)
			run = reduce(&stack, data->p_table, entry, &result);
		else if (entry && entry->action == A_ACCEPT)
			run = parse_accept(data, stack, result);
		else
			run = parse_reject(data, stack, result);
	}
	if (run == -1)
		return (free_stack(&result), free_stack(&stack),
			safe_exit(data, MALLOC_FAIL));
}
