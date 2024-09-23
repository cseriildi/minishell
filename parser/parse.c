/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 16:03:07 by pvass             #+#    #+#             */
/*   Updated: 2024/09/23 12:56:34 by icseri           ###   ########.fr       */
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
	printf ("stack->state: %d, input_type %d\n", stack->state, input_type);
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
		printf("ty:%d\n", temp->type);
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
	t_token	*token_list;
	t_stack	*result;
	int		run;

	data->p_table = create_table();
	if (data->p_table == NULL)
		return ;
	printf("kurva3\n");
	token_list = data->tokens;
	run = TRUE;
	stack = init_stack();
	result = NULL;
	if (stack == NULL)
		return ;
	while (run)
	{
		//printf("aaaaaaaaaaaaaaaa\n");
		//print_stack(stack);
		entry = get_entry(token_list, data->p_table, stack);
		//printf("aaaaaaaaaaaaaaaa%p\n", entry);
		//printf("pentry: %p\n", entry);
		//if(entry != NULL)
			//printf("entry:%d\n", entry->action);
		if (entry && entry->action == A_SHIFT)
			run = shift(&stack, &token_list, entry->next_s);
		else if (entry && entry->action == A_REDUCE)
			run = reduce(&stack, data->p_table, entry, &result);
		else if (entry && entry->action == A_ACCEPT)
		{
			run = 0;
			/* printf("result\n");
			print_stack(result); */
			put_pipes_right_place(&result);
			/* printf("stack\n");
			print_stack(stack);
			printf("result\n");
			print_stack(result); */
			
			data->exec = create_exec(&result);
			if (data->exec == NULL)
				break ;
			//print_stack(result);
			//printf("\nFINAL\n\n");
			//print_exec(exec);
			//printf("\nSTART REVERSING\n\n");
			reverse_exec(&data->exec);
			//printf("\nFINAL after reverse\n\n");
			print_exec(data->exec);
			
			printf("ACCEPT\n");
		}
		else
		{
			run = 0;
			//print_stack(stack);
			printf("REJECT\n");
		}
	}
	//print_stack(result);
	free_stack(&result);
	//print_stack(stack);
	free_stack(&stack);
	//printf("ENDING\n");
	/* free_exec_all(&exec); */
}

/* int main ()
{
	t_token *token;
	t_table	*p_table;

	token = create_new_token("1st word", HERE_DOC);
	add_token_to_back(&token, create_new_token("EOF", WORD));
	add_token_to_back(&token, create_new_token("EOF", WORD));
	add_token_to_back(&token, create_new_token("EOF", PIPE));
	add_token_to_back(&token, create_new_token("cat", L_BRACKET));
	add_token_to_back(&token, create_new_token("ls", WORD));
	add_token_to_back(&token, create_new_token("|", OR));
	add_token_to_back(&token, create_new_token("ls", WORD));
	add_token_to_back(&token, create_new_token("cat", R_BRACKET));
	add_token_to_back(&token, create_new_token(">>", PIPE));
	add_token_to_back(&token, create_new_token("ls", WORD));
	add_token_to_back(&token, create_new_token("|", APPEND));
	add_token_to_back(&token, create_new_token("aaaa", WORD));
	add_token_to_back(&token, create_new_token("END", END));
	p_table = create_table();
	parse(p_table, token);
	return (0);
} */