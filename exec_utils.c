/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:50:42 by pvass             #+#    #+#             */
/*   Updated: 2024/10/25 10:57:03 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token **tokens)
{
	t_token	*head;
	t_token	*current;

	head = *tokens;
	current = head;
	while (current != NULL)
	{
		ft_printf("%s\n", current->content);
		ft_printf("%d\n", current->type);
		current = current->next;
	}
}

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
	ft_printf("PRINTING EXEC:\n");
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

t_exec	*create_exec_node(char *content)
{
	t_stack	*res;
	t_exec	*exec;

	res = malloc(sizeof(t_stack));
	if (!res)
		return (NULL);
	res->data = ft_strdup(content);
	if (!res->data)
		return (NULL);
	res->state = 0;
	res->type = WORD;
	res->next = NULL;
	exec = exec_new(&res);
	if (!exec)
	{
		free(res->data);
		free(res);
		return (NULL);
	}
	free(res->data);
	free(res);
	return (exec);
}
