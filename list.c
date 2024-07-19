/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:57:32 by icseri            #+#    #+#             */
/*   Updated: 2024/07/19 17:24:58 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_new_token(char *content, int type)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return (NULL);
	new_node->content = ft_strdup(content);
	if (new_node->content == NULL)
		return (free(new_node), NULL);
	new_node->type = type;
	new_node->next = NULL;
	return (new_node);
}

void	add_token_to_back(t_token **lst, t_token *new)
{
	t_token	*curr;

	if (lst && new)
	{
		if (*lst == NULL)
			*lst = new;
		else
		{
			curr = *lst;
			while (curr->next)
				curr = curr->next;
			curr->next = new;
		}
	}
}

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return ;
	if (*tokens)
	{
		current = *tokens;
		while (current != NULL)
		{
			next = current->next;
			free(current->content);
			free(current);
			current = next;
		}
	}
	free(tokens);
}
