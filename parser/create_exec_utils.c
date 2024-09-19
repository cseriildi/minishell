/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exec_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 10:23:37 by pvass             #+#    #+#             */
/*   Updated: 2024/09/19 15:02:54 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_exec	*exec_new(t_stack **res)
{
	t_exec	*new_node;
	int		new_type;
	int		run;

	run = 1;
	new_type = 0;
	new_node = malloc(sizeof(t_exec));
	printf("HAHAHA\n");
	print_stack(*res);
	if (new_node == NULL)
		return (NULL);
	new_node->down = NULL;
	new_node->next = NULL;
	printf("HAHAHAaaaaaa\n");
	if ((*res)->type == 6)
	{
		new_node->type = (*res)->type;
		if ((*res)->data != NULL)
		{
			new_node->data = ft_strdup((*res)->data);
			if (new_node->data == NULL)
				return (free(new_node), NULL);
		}
		else 
			new_node->data = NULL;
		return (new_node);
	}
	while (*res != NULL && run == 1)
	{
		if ((*res)->type < 100 && (*res)->type > new_type)
			new_type = (*res)->type;
		if ((*res)->type == 2)
		{
			run = 0;
			new_node->data = ft_strdup((*res)->data);
			if (new_node->data == NULL)
				return (free(new_node), NULL);
			break ;
		}
		*res = (*res)->next;
	}
	//printf("HAHAHAaaa%p%s\n", new_node, new_node->data);
	new_node->type = new_type;
	return (new_node);
}

void	exec_add_back(t_exec **where_a, t_exec *what)
{
	t_exec *where;

	where = *where_a;
	if (where_a == NULL)
		return;
	if (where == NULL)
	{
		*where_a = what;
		return;
	}
	while (where->next != NULL)
		where = where->next;
	where->next = what;
}

void	exec_add_under(t_exec **where_a, t_exec *what)
{
	t_exec *where;

	where = *where_a;
	if (where_a == NULL)
		return;
	if (where == NULL)
	{
		*where_a = what;
		return ;
	}
	while (where->down != NULL)
		where = where->down;
	where->down = what;
}

void	exec_add_back_under(t_exec **where_a, t_exec *what)
{
	t_exec *where;

	where = *where_a;
	if (where_a == NULL)
		return;
	//printf("HALO4 %p\n", where);
	if (where == NULL)
	{
		*where_a = what;
		//printf("exit here\n");
		return ;
	}
	//printf("HALO4.2 %s, %p\n", where->data, where->next);
	while (where->next != NULL)
		where = where->next;
	//exec_add_under(&where, what);
	while (where->down != NULL)
		where = where->down;
	where->down = what;
}

void	print_exec(t_exec *exec)
{
	t_exec	*temp;
	t_exec	*temp2;
	
	temp = exec;
	temp2 = exec;
	printf("PRINTING EXEC:\n");
	while (temp != NULL)
	{
		temp2 = temp;
		while (temp2 != NULL)
		{
			printf("Exec: TYPE: %d, DATA:%s\n", temp2->type, temp2->data);
			temp2 = temp2->down;
		}
		printf("NEXT\n");
		temp = temp->next;
	}
}

int exec_last_is_not_cmd(t_exec *exec)
{
	int	res;

	res = 0;
	if (exec == NULL)
		return (1);
	while (exec->next != NULL)
		exec = exec->next;
	if(exec->type == 2)
		return (0);
	return (1);	
}

t_exec	*create_exec(t_stack **res)
{
	t_exec	*new;
	t_exec	*temp;
	t_stack	*temp2;

	new = NULL;
	printf("HALO\n");
	temp2 = *res;
	while (temp2 != NULL)
	{
		temp = exec_new(&temp2);
		if (temp == NULL)
			return (free_exec_all(&new), NULL);
		print_exec(temp);
		printf("HALO22222222%p\n", temp);
		if (temp == NULL)
			return (NULL);
		if (temp->type != WORD || exec_last_is_not_cmd(new) == 1)
			exec_add_back(&new, temp);
		else
		{
			//printf("\n\nBEFORE\n\n");
			//print_exec(new);
			exec_add_back_under(&new, temp);
			//printf("\n\nAFTER\n%p %p\n", new, temp);
			//print_exec(new);
		}
		//printf("NEWWWWWWWWWWWWWWWW\n");
		print_exec(new);
		temp2 = temp2->next;
	}
	//printf("HALO3\n");
	return (new);
}
