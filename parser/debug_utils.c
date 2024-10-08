/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:19 by pvass             #+#    #+#             */
/*   Updated: 2024/07/20 17:28:16 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	print_stack(t_stack *stack)
{
	int	i;

	i = 0;
	while (stack != NULL)
	{
		ft_printf("Element%d: type: %d, data:%s, state: %d\n", i, stack->type,
			stack->data, stack->state);
		i++;
		stack = stack->next;
	}
	ft_printf("\n");
}
