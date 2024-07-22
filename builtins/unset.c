/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:53:04 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 18:55:06 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	del_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (env && *env && key && *key)
	{
		current = *env;
		prev = NULL;
		while (current != NULL)
		{
			if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			{
				if (prev == NULL)
					*env = current->next;
				else
					prev->next = current->next;
				ft_free(&current->content);
				ft_free(&current->key);
				free(current);
				break ;
			}
			prev = current;
			current = current->next;
		}
	}
}

void	ft_unset(t_var *data, t_ast *token)
{
	del_var(data->env, token->data);
	exit(EXIT_SUCCESS);
}
