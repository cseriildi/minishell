/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:26:11 by pvass             #+#    #+#             */
/*   Updated: 2024/09/17 14:40:19 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	swap_int(int *a, int *b)
{
	int	t;

	t = *a;
	*a = *b;
	*b = t;
}

void	swap_str(char **a, char**b)
{
	char *t;

	t = *a;
	*a = *b;
	*b = t;
}
void	swap_stack(t_stack **a, t_stack **b)
{
	swap_int(&((*a)->state), &((*b)->state));
	swap_int(&((*a)->type), &((*b)->type));
	swap_str(&((*a)->data), &((*b)->data));
}