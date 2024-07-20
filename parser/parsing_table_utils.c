/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_table_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:14:59 by pvass             #+#    #+#             */
/*   Updated: 2024/07/20 15:10:18 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

t_table	*table_new(char **args)
{
	t_table	*new;

	new = (t_table *) malloc (sizeof(t_table));
	if (new == NULL)
		return (NULL);
	new->state = ft_atoi(args[0]);
	new->event = ft_atoi(args[1]);
	new->action = ft_atoi(args[2]);
	new->next_s = ft_atoi(args[3]);
	new->nb_reduce = ft_atoi(args[4]);
	new -> next = NULL;
	return (new);
}

void	table_add_back(t_table **lst, t_table *new)
{
	t_table	*i;

	i = *lst;
	if (new == NULL || lst == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while (i -> next != NULL)
	{
		i = i -> next;
	}
	i -> next = new;
}

void	table_clear(t_table **lst)
{
	t_table	*i;
	t_table	*temp;

	i = *lst;
	if (lst == NULL  || *lst == NULL)
		return ;
	while (i != NULL)
	{
		temp = i -> next;
		free(i);
		i = temp;
	}
	*lst = NULL;
}

void	free_tab(char **tabb)
{
	int	i;
	
	i = 0;
	while (tabb[i] != NULL)
	{
		free(tabb[i]);
		i++;
	}
	free(tabb);
}
/*NEW*/

