/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrange_exec_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:12:57 by pvass             #+#    #+#             */
/*   Updated: 2024/09/27 12:40:32 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_exec	*reverse_exec_util(t_exec **exec)
{
	t_exec	*next_temp;
	t_exec	*temp;
	t_exec	*prev;

	next_temp = *exec;
	prev = NULL;
	if (next_temp != NULL)
	{
		reverse_down_up(&next_temp);
		*exec = next_temp;
		prev = next_temp;
		temp = *exec;
		*exec = (*exec)->next;
		next_temp = next_temp->next;
	}
	while (next_temp != NULL)
	{
		reverse_down_up(&next_temp);
		*exec = next_temp;
		prev->next = next_temp;
		prev = prev->next;
		*exec = (*exec)->next;
		next_temp = next_temp->next;
	}
	return (temp);
}

void	reverse_exec(t_exec **exec)
{
	t_exec	*temp;

	if (exec == NULL || *exec == NULL)
		return ;
	temp = reverse_exec_util(exec);
	*exec = temp;
	reverse_front_back(exec);
	arrange_in_pip_seq(exec);
	temp = *exec;
	sort_in_pip_seq(exec);
	sort_in_pip_seq(exec);
	while (temp != NULL)
	{
		sort_in_pip_seq(&temp);
		temp = temp->next;
	}
}
