/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:49 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 18:55:00 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_env(t_var *data)
{
	t_env	*head;
	t_env	*curr;

	head = *data->env;
	curr = head;
	while (curr != NULL)
	{
		printf("%s=%s\n", curr->key, curr->content);
		curr = curr->next;
	}
	exit(EXIT_SUCCESS);
}
