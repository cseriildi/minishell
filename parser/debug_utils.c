/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:19 by pvass             #+#    #+#             */
/*   Updated: 2024/07/20 15:58:10 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	print_stack(t_stack *stack)
{
	int	i;

	i = 0;
	while (stack != NULL)
	{
		printf("Element%d: type: %d, data:%s, state: %d\n", i, stack->type, stack->data, stack->state);
		i++;
		stack = stack->next;
	}
	printf("\n");
}
