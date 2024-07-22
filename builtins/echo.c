/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 15:47:19 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_echo(t_var *data, t_ast *token)
{
	(void)data;
	if (token->data && ft_strncmp(token->data, "-n", 3) == 0)
	{
		if (token->next->data)
			printf("%s%%\n", token->next->data);
	}
	else if (token->data)
		printf("%s\n", token->data);
	else
		printf("%s\n", "");
	exit(EXIT_SUCCESS);
}
