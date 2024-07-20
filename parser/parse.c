/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 16:03:07 by pvass             #+#    #+#             */
/*   Updated: 2024/07/20 15:19:16 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

t_table *get_entry(t_token *token, t_table *p_table, t_stack *stack)
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
		//printf("p_table->state: %d\nstack->state: %d\n", p_table->state, stack->state);
		if (p_table->state == stack->state)
		{
			//printf("p_table->event: %d\nstack->event: %d\n", p_table->event, input_type);
			if (p_table->event == input_type)
				return (p_table);
			else if (p_table->event == P_NONE)
				table_entry = p_table;
		}
		p_table = p_table->next;
	}
	return (table_entry);
}

int get_next_state(t_table *p_table, t_stack *stack)
{
	int	next_state;

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

void	parse(t_table *p_table, t_token **tokens)
{
	t_stack *stack;
	t_table	*entry;
	t_token *token_list;
	int		run;

	token_list = *tokens;
	run = TRUE;
	stack = init_stack();
	if (stack == NULL)
		return ;
	while (/* token_list != NULL  &&*/ run)
	{
		//print_stack(stack);
		entry = get_entry(token_list, p_table, stack);
		//print_p_table(entry);
		//print_stack(stack);
		if (entry && entry->action == A_SHIFT)
			run = shift(&stack, &token_list, entry->next_s);
		else if (entry && entry->action == A_REDUCE)
			run = reduce(&stack, p_table, entry);
		else if (entry && entry->action == A_ACCEPT)
		{
			run = 0;//accept();
			printf("ACCEPT\n");
			//print_stack(stack);
			//printf("token: %s\n", token_list->content);
		}
		else
		{
			run = 0;//reject();
			printf("REJECT\n");
			//print_stack(stack);
		}
		//printf ("run:%d\n", run);
		//print_stack(stack);
		//printf("---------------\n");
		//if (entry)
		//	printf("%d	%d	%d	%d	%d\n", entry->state, entry->event, entry->action, entry->next_s, entry->nb_reduce);
		//token_list = token_list->next;
	}
	//print_p_table(entry);
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