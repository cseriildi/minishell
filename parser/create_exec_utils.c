/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exec_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 10:23:37 by pvass             #+#    #+#             */
/*   Updated: 2024/09/27 13:08:18 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_exec	*exec_return_pipe(t_exec *new_node, t_stack **res)
{
	new_node->type = (*res)->type;
	if ((*res)->data != NULL)
	{
		new_node->data = ft_strdup((*res)->data);
		if (new_node->data == NULL)
			return (free(new_node), new_node = NULL, NULL);
	}
	else
		new_node->data = NULL;
	return (new_node);
}

t_exec	*exec_return_others(t_stack **res, t_exec *new_node)
{
	int		new_type;
	int		run;

	run = 1;
	new_type = 0;
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
	new_node->type = new_type;
	return (new_node);
}

t_exec	*exec_new(t_stack **res)
{
	t_exec	*new_node;

	new_node = malloc(sizeof(t_exec));
	if (new_node == NULL)
		return (NULL);
	new_node->down = NULL;
	new_node->next = NULL;
	if ((*res)->type == 6)
		return (exec_return_pipe(new_node, res));
	else
		return (exec_return_others(res, new_node));
}

void	exec_add_back(t_exec **where_a, t_exec *what)
{
	t_exec	*where;

	where = *where_a;
	if (where_a == NULL)
		return ;
	if (where == NULL)
	{
		*where_a = what;
		return ;
	}
	while (where->next != NULL)
		where = where->next;
	where->next = what;
}

void	exec_add_under(t_exec **where_a, t_exec *what)
{
	t_exec	*where;

	where = *where_a;
	if (where_a == NULL)
		return ;
	if (where == NULL)
	{
		*where_a = what;
		return ;
	}
	while (where->down != NULL)
		where = where->down;
	where->down = what;
}
