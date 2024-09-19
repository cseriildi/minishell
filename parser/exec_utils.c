/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:50:42 by pvass             #+#    #+#             */
/*   Updated: 2024/09/19 14:53:46 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
		curr = curr->next;
		if (prev->data != NULL)
			free(prev->data);
		free(prev);
	}
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
	}
}