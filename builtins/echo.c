/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/07/20 17:30:09 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	echo(t_var *data, t_ast *token)
{
	if (ft_strncmp(token->data, "-n", 3) == 0)
		printf("%s\n", token->next->data);
	printf("%s", token->data);
	safe_exit(data, EXIT_SUCCESS);
}