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
	new = (t_stack *) malloc (sizeof(t_stack *));
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
		if (temp_stack1->next->next->type == 6)
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
		else if (temp_stack1->type == 2 && temp_stack1->next->type != 2 && new != NULL)
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

void	transform(t_stack *result, t_stack *stack)
{
	stack_reverse(&result);
	stack_reverse(&stack);
	ft_printf("res:\n");
	print_stack(result);
	ft_printf("stack:\n");
	stack = stack->next;
	print_stack(stack);


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
			new_temp = dup_stack_element(stack);
			if (new_temp == NULL)
				return ;
			stack_add_front(&new, new_temp);
			//print_stack(new);
		}
		else if (stack->type > 100)
		{
			//ft_printf("else:\n");
			new_temp = dup_stack_element(stack);
			if (new_temp == NULL)
				return ;
			stack_add_front(&new, new_temp);
			//ft_printf("else2:\n");
			new_temp2 = get_elem_stack(&result);
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
	stack_rearr_pipes(&new);
	stack_reverse(&new);
	ft_printf("NEW STACK AFTER REARR\n");
	print_stack(new);
}