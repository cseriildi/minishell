/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrange_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:06:22 by pvass             #+#    #+#             */
/*   Updated: 2024/09/26 13:20:22 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	reverse_down_up(t_exec **exec)
{
	t_exec	*curr;
	t_exec	*prev;
	t_exec	*next;
	t_exec	*first;

	first = *exec;
	prev = NULL;
	curr = *exec;
	next = NULL;
	if (curr->down == NULL)
		return ;
	while (curr != NULL)
	{
		next = curr->down;  
        curr->down = prev;  
        prev = curr;        
        curr = next;
	}
	prev->next = first->next;
	first->next = NULL;
	*exec = prev;
	
}

void	reverse_front_back(t_exec **exec)
{
	t_exec	*curr;
	t_exec	*prev;
	t_exec	*next;
	t_exec	*first;

	first = *exec;
	prev = NULL;
	curr = *exec;
	next = NULL;
	if (curr->next == NULL)
		return ;
	while (curr != NULL)
	{
		next = curr->next;  
        curr->next = prev;  
        prev = curr;        
        curr = next;
	}
	*exec = prev;
	
}

void	arrange_in_pip_seq(t_exec **exec)
{
	t_exec	*curr;
	t_exec	*prev;

	curr = *exec;
	if (exec == NULL || *exec == NULL)
		return ;
	while (curr->next != NULL)
	{
		prev = curr;
		curr = curr-> next;
		if (prev->type != PIPE && curr->type != PIPE)
		{
			prev->next = curr->next;
			curr->next = NULL;
			exec_add_under(&prev, curr);
			curr = prev;
		}
		if (curr->type == PIPE)
		{
			prev->next = curr->next;
			free (curr);
			curr = prev->next;
		}
	}
}
void	sort_in_pip_seq_util(t_exec **prev_d, t_exec **curr_d)
{
	t_exec	*prev;
	t_exec	*curr;
	int		t;
	char	*temp;

	curr = *curr_d;
	prev = *prev_d;
	t = prev->type;
	prev->type = curr->type;
	curr->type = t;
	temp = prev->data;
	prev->data = curr->data;
	curr->data = temp;
}

void	sort_in_pip_seq(t_exec **exec)
{
	t_exec	*prev;
	t_exec	*curr;
	t_exec	*exec_temp;

	if (exec == NULL || *exec == NULL)
		return ;
	exec_temp = *exec;
	curr = exec_temp;
	while (exec_temp->down != NULL)
	{
		curr = exec_temp;
		while (curr->down != NULL)
		{
			prev = curr;
			curr = curr->down;
			if (curr->type == 2 && prev->type != 2)
				sort_in_pip_seq_util(&prev, &curr);
		}
		exec_temp = exec_temp->down;
	}
}

void	reverse_exec(t_exec **exec)
{
	t_exec *next_temp;
	t_exec *temp;
	t_exec *prev;
	int	first;
	
	if (exec == NULL || *exec == NULL)
		return ;
	next_temp = *exec;
	first = TRUE;
	prev = NULL;
	while (next_temp != NULL)
	{
		reverse_down_up(&next_temp);
		*exec = next_temp;
		if (first == TRUE)
		{
			prev = next_temp;
			temp = *exec;
			first = FALSE;
		}
		else
		{
			prev->next = next_temp;
			prev = prev->next;
		}
		*exec = (*exec)->next;
		next_temp = next_temp->next;
	}
	*exec = temp;
	reverse_front_back(exec);
	arrange_in_pip_seq(exec);;
	temp = *exec;
	sort_in_pip_seq(exec);
	sort_in_pip_seq(exec);
	while (temp != NULL)
	{
		sort_in_pip_seq(&temp);
		temp = temp->next;
	}
}
