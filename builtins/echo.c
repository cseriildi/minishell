/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 15:41:23 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_echo(t_var *data, t_ast *token)
{
	if (token->data && ft_strncmp(token->data, "-n", 3) == 0)
	{
		if (token->next->data)
			printf("%s", token->next->data);
	}
	else if (token->data)
		printf("%s\n", token->data);
	data->exit_code = EXIT_SUCCESS;
}
