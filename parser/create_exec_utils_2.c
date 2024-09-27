/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exec_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:19:28 by pvass             #+#    #+#             */
/*   Updated: 2024/09/27 13:07:03 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	exec_add_back_under(t_exec **where_a, t_exec *what)
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
	while (where->down != NULL)
		where = where->down;
	where->down = what;
}

int	exec_last_is_not_cmd(t_exec *exec)
{
	if (exec == NULL)
		return (1);
	while (exec->next != NULL)
		exec = exec->next;
	if (exec->type == 2)
		return (0);
	return (1);
}

t_exec	*create_exec(t_stack **res)
{
	t_exec	*new;
	t_exec	*temp;
	t_stack	*temp2;

	new = NULL;
	temp2 = *res;
	while (temp2 != NULL)
	{
		temp = exec_new(&temp2);
		if (temp == NULL)
			return (free_exec_all(&new), NULL);
		if (temp->type != WORD || exec_last_is_not_cmd(new) == 1)
			exec_add_back(&new, temp);
		else
			exec_add_back_under(&new, temp);
		temp2 = temp2->next;
	}
	return (new);
}
