/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:44:24 by icseri            #+#    #+#             */
/*   Updated: 2024/07/20 17:19:41 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	builtin(t_var *data, t_ast *token)
{
	if (ft_strncmp(token->data, "cd", 3) == 0)
		cd(data, token->child);
	else if (ft_strncmp(token->data, "echo", 5) == 0)
		echo(data, token->child);
	else if (ft_strncmp(token->data, "env", 5) == 0)
		echo(data, token->child);
	else if (ft_strncmp(token->data, "exit", 5) == 0)
		echo(data, token->child);
	else if (ft_strncmp(token->data, "export", 5) == 0)
		echo(data, token->child);
	else if (ft_strncmp(token->data, "pwd", 5) == 0)
		echo(data, token->child);
	else if (ft_strncmp(token->data, "unset", 5) == 0)
		echo(data, token->child);
}