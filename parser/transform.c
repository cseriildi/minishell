# include "parser.h"

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

t_stack *dup_stack_element(t_stack *old)
{
	t_stack *new;

	//printf("zzz%ld\n", sizeof(t_stack *));
	new = (t_stack *) malloc (sizeof(t_stack));
	//printf("zzz%ld\n", sizeof(t_stack *));
	if (new == NULL)
		return (NULL);
	new->data = old->data;
	new->next = NULL;
	new->state = old->state;
	new->type = old->type;
	return (new);
}

t_stack	*get_elem_stack(t_stack	**res)
{
	t_stack *new_temp;
	t_stack *new;
	t_stack	*temp;

	new = NULL;
	temp = *res;
	while (temp->data == NULL)
		temp = temp->next;
	new_temp = dup_stack_element(temp);
	stack_add_front(&new, new_temp);
	temp = temp->next;
	while (temp-> next != NULL && temp->type != 2)
	{
		if (temp->type > 1)
		{
			new_temp = dup_stack_element(temp);
			stack_add_front(&new, new_temp);
		}
		temp = temp-> next;
	}
	*res = temp;
	return (new);
}

t_stack *get_cmd_suffix(t_stack **res)
{
	t_stack *new_temp;
	t_stack *new;
	t_stack	*temp;
	t_stack *temp2;
	t_stack *unused;

	temp = *res;
	unused = NULL;
	new = NULL;
	ft_printf("itt0\n");
	print_stack(temp);
	while (temp->type != P_CMD && temp != NULL && temp->next != NULL && temp->next->next!=NULL)
	{
		if (temp->type > 1)
		{
			temp2 = temp->next->next;
			if ((temp2->type > 108 && temp->type == P_WORD) || temp->type > 108)
			{
				new_temp = dup_stack_element(temp);
				stack_add_front(&unused, new_temp);
			}
			else
			{
				new_temp = dup_stack_element(temp);
				stack_add_front(&new, new_temp);
			}
		}
		ft_printf("NEW:\n");
		print_stack(new);
		ft_printf("UNUSED:\n");
		print_stack(unused);
		temp = temp->next;
	}
	ft_printf("itt2\n");
	if (temp->type == P_CMD)
	{
		new_temp = dup_stack_element(temp);
		stack_add_front(&new, new_temp);
		temp = temp->next;
	}
	ft_printf("itt3\n");
	if (unused != NULL)
	{
		while(unused->next != NULL)
			unused = unused->next;
		unused->next = temp;
		*res = unused;
	}
	ft_printf("itt4\n");
	return(new);
}
void	stack_rearr_pipes(t_stack **stack)
{
	t_stack	*temp_stack1;
	t_stack *temp_stack2;
	t_stack *temp;
	t_stack *new;

	new = NULL;
	temp_stack1 = *stack;
	ft_printf("BEGIN:\n");
	print_stack(*stack);
	while (temp_stack1 != NULL && temp_stack1->next != NULL && temp_stack1->next->next!= NULL)
	{
		//ft_printf("PROCESS:\n");
		//print_stack(*stack);
		if (temp_stack1->next->next->type == PIPE && temp_stack1->next->type == P_PIPE_SEQ)
		{
			//ft_printf("HALLO ELJUTOTTAM IDAIG\n");
			temp = dup_stack_element(temp_stack1->next);
			stack_add_front(&new, temp);
			//ft_printf("HALLO ELJUTOTTAM 2\n");

			temp = dup_stack_element(temp_stack1->next->next);
			stack_add_front(&new, temp);
			temp_stack2 = temp_stack1->next->next->next;
			temp_stack1->next = temp_stack2;
			//ft_printf("NEW:\n");
			//print_stack(new);
		}
		else if (temp_stack1->next->type == PIPE && temp_stack1->next->next->type != P_WORD)
		{
			temp = dup_stack_element(temp_stack1->next);
			stack_add_front(&new, temp);
			temp_stack2 = temp_stack1->next->next;
			temp_stack1->next = temp_stack2;
		}
		else if (temp_stack1->type == P_WORD && temp_stack1->next->type != P_WORD && new != NULL)
		{
			temp_stack2 = temp_stack1->next;
			temp_stack1->next = new;
			while (temp_stack1->next != NULL)
				temp_stack1 = temp_stack1->next;
			temp_stack1->next = temp_stack2;
			temp_stack1 = new;
			new = NULL;
		}
		else
			temp_stack1 = temp_stack1->next;
	}
}

void	rev_stack_mid(t_stack **stack_from, t_stack **stack_until)
{
	t_stack *from;
	t_stack *until;
	t_stack *new;
	t_stack *new_temp;

	from = *stack_from;
	until = *stack_until;
	new = until;
	while(from != until)
	{
		new_temp = dup_stack_element(from);
		if (new_temp == NULL)
			return;
		stack_add_front(&new, new_temp);
		from = from->next;
	}
	(*stack_from)->next = new;
}

void	stack_to_res_one_pipe(t_stack **stack_d, t_stack **res_d)
{
	t_stack	*stack;
	t_stack *res;
	t_stack *res_rest;
	t_stack *temp;
	t_stack *stack_temp;

	stack = *stack_d;
	res = *res_d;
	if (stack->next->next == NULL)
		return ;
	while (res->type != 102 && res->next != NULL)
		res = res->next;
	if (res->type == 102)
	{
		res_rest = res->next;
		while ((stack->type != P_PIPE_SEQ && stack->next->next->type != P_PIPE) && stack->next->next->next != NULL)
			stack = stack->next;
		stack_temp = *stack_d;
		while (stack_temp->next != stack)
			stack_temp = stack_temp->next;
		if (stack != NULL)
		{
			temp = stack;
			stack_temp->next = stack->next->next->next->next;
			temp->next->next->next->next = res_rest;
			res->next = temp;
			//*stack_d = stack; 
		}
		while (res != NULL && res->next != NULL && res->next->next != NULL && res->next->next->next != NULL)
		{
			if (res->next->type == P_PIPE && res->next->next->next->type == P_PIPE_SEQ)
			{
				rev_stack_mid(&res, &(res->next->next->next->next));
				//ft_printf("HALLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n");
			}
			res = res->next;
		}
	}
}

void	stack_rearr_parts(t_stack **stacks)
{
	t_stack *stack;
	t_stack	*temp;
	int		count;

	stack = *stacks;
	while (stack != NULL && stack->type != 5 && stack->type != 4 && stack->type != 7 && stack->type != 8)
		stack = stack->next;
	if (stack == NULL)
		return ;
	temp = stack;
	while (stack != NULL)
	{
		count = 0;
		if (stack->type < 100)
			count++;
		if (count == 2)
		{
			rev_stack_mid(&stack, &temp);
			
		}
		temp = temp->next;
	}
}

void	result_rearr_pipes(t_stack **res)
{
	t_stack	*temp;
	t_stack *new;
	t_stack	*new_temp;
	t_stack	*temp2;

	temp = *res;
	new = NULL;
	while (temp != NULL && temp->next->next != NULL)
	{
		if (temp->type == P_CMD && temp->next->next->type != P_PIPE_SEQ)
		{
			temp2 = temp;
			while (temp2->next->next->next != NULL && temp2->next->type != P_PIPE)
				temp2 = temp2->next;
			if (temp2->next->type == P_PIPE)
			{
				new_temp = dup_stack_element(temp2->next);
				stack_add_front(&new, new_temp);
				new_temp = dup_stack_element(temp2->next->next->next);
				stack_add_front(&new, new_temp);
				temp2->next = temp2->next->next->next->next;
				new->next->next = temp->next;
				temp->next = new;
			}
		}
		temp = temp->next;
	}
}

int	look_for_pipe(t_stack *stack)
{
	while (stack != NULL)
	{
		if (stack->type == 6)
			return (1);
		stack = stack -> next;
	}
	return (0);
}

int	stack_to_res(t_stack *stack, t_stack *res)
{
	int	count;

	count = 0;
	if (stack->type == 5 || stack->type == 4 || stack->type == 7 || stack->type == 8)
	{
		while (res != NULL)
		{
			if (res->type == 2)
				count++;
			if ((res->type == 112 || res->type == 114) && count == 1)
				return (1);
			res = res->next;
		}
		return (0);
	}
	else
		return (1);
}

t_stack *create_temp_stack()
{
	t_stack *new;

	new = (t_stack *) malloc (sizeof(t_stack));
	if (new == NULL)
		return NULL;
	new->data = NULL;
	new->type = 108;
	new->state = 0;
	new->next = NULL;
	return (new);
}

void	transform(t_stack *result, t_stack *stack)
{
	stack_reverse(&result);
	stack_reverse(&stack);
	ft_printf("res:\n");
	print_stack(result);
	if (look_for_pipe(stack) == 0)
		result_rearr_pipes(&result);
	else
		stack_to_res_one_pipe(&stack, &result);

	ft_printf("after_rearr:\n");
	print_stack(result);
	ft_printf("stack:\n");
	stack = stack->next;
	print_stack(stack);
	//return ;

	t_stack	*new;
	t_stack	*new_temp;
	t_stack *new_temp2;

	new = NULL;
	new_temp2 = NULL;
	while (stack != NULL)
	{
		//ft_printf("resssss:\n");
		if (stack->type == 0 || (stack->type > 1 && stack->type < 100))
		{
			if (stack_to_res(stack, result) == 1)
			{
				new_temp = dup_stack_element(stack);
				if (new_temp == NULL)
					return ;
				stack_add_front(&new, new_temp);
			}
			else
			{
				new_temp = create_temp_stack();
				if (new_temp == NULL)
					return ;
				stack_add_front(&stack, new_temp);
				new_temp = create_temp_stack();
				if (new_temp == NULL)
					return ;
				stack_add_front(&stack, new_temp);
			}

			//print_stack(new);
		}
		else if (stack->type > 100 )
		{
			//ft_printf("else:\n");
			new_temp2 = get_elem_stack(&result);
			stack_reverse(&new_temp2);
			//ft_printf("new_teemp2:\n");
			print_stack(new_temp2);
			while (new_temp2 != NULL)
			{
				//ft_printf("bbmaaaaa\n");
				new_temp = dup_stack_element(new_temp2);
				//ft_printf("aaaaaaaa\n");
				if (new_temp == NULL)
					return ;
				stack_add_front(&new, new_temp);
				new_temp2 = new_temp2->next;
			}
			new_temp = dup_stack_element(stack);
			if (new_temp == NULL)
				return ;
			stack_add_front(&new, new_temp);
			//ft_printf("else2:\n");
			if (stack->type == P_CMD_SUFFIX /*|| stack->type == P_CMD_NAME*/) /*add the rest of suffixes to the new stack*/
			{
				ft_printf("here\n");
				new_temp2 = get_cmd_suffix(&result);
				ft_printf("here1:\n");
				print_stack(new_temp2);
				stack_reverse(&new_temp2);
				while (new_temp2 != NULL)
				{
					new_temp = dup_stack_element(new_temp2);
					if (new_temp == NULL)
						return ;
					stack_add_front(&new, new_temp);
					new_temp2 = new_temp2->next;
				}
				ft_printf("here2\n");
				print_stack(stack);
				print_stack(result);
				print_stack(new);
			}
		}
		else if (stack->type == 100)
		{
			//ft_printf("100\n");
			while (result != NULL)
			{
				//ft_printf("nem ertem\n");
				if (result->type != 1)
				{
					//ft_printf("maradtdupelott:\n");
					//print_stack(result);
					new_temp = dup_stack_element(result);
					stack_add_front(&new, new_temp);
					//ft_printf("igy allunk:\n");
					//print_stack(new);
				}
				new_temp2 = result;
				result = result->next;
				free(new_temp2);
				//ft_printf("maradt:\n");
				//print_stack(result);
			}
			//print_stack(new);
		}
		stack = stack->next;
	}
	ft_printf("NEW STACK\n");
	print_stack(new);
	//return ;
	//stack_rearr_pipes(&new);
	//stack_rearr_parts(&new);
	stack_reverse(&new);
	ft_printf("NEW STACK AFTER REARR\n");
	print_stack(new);
}