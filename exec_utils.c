/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:50:42 by pvass             #+#    #+#             */
/*   Updated: 2024/09/25 13:36:44 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_down_exec(t_exec **exec)
{
	t_exec	*prev;
	t_exec	*curr;

	curr = *exec;
	if (exec == NULL)
		return ;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->down;
		if (prev->data != NULL)
		{
			ft_free(&prev->data);
			prev->data = NULL;
		}
		free(prev);
		prev = NULL;
	}
	*exec = NULL;
}
void	free_exec_all(t_exec **exec)
{
	t_exec	*prev;
	t_exec	*curr;

	curr = *exec;
	if (exec == NULL)
		return ;
	if (*exec == NULL)
		return /* (free(exec)) */;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		free_down_exec(&prev);
		prev = NULL;
	}
	*exec = NULL;
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
			ft_printf("Exec: TYPE: %d, DATA:%s\n", temp2->type, temp2->data);
			temp2 = temp2->down;
		}
		ft_printf("NEXT\n");
		temp = temp->next;
	}
}
