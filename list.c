/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:57:32 by icseri            #+#    #+#             */
/*   Updated: 2024/07/17 14:20:59 by icseri           ###   ########.fr       */
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
	{
		free(new_node);
		return (NULL);
	}
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
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]->content);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
