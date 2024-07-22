/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:44:24 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 15:12:17 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	builtin(t_var *data, t_ast *token)
{
	if (ft_strncmp(token->data, "cd", 3) == 0)
		ft_cd(data, token->child);
	else if (ft_strncmp(token->data, "echo", 5) == 0)
		ft_echo(data, token->child);
	else if (ft_strncmp(token->data, "env", 4) == 0)
		ft_env(data);
	else if (ft_strncmp(token->data, "exit", 5) == 0)
		ft_exit(data);
	else if (ft_strncmp(token->data, "export", 7) == 0)
		ft_export(data, token->child);
	else if (ft_strncmp(token->data, "pwd", 4) == 0)
		ft_pwd(data);
	else if (ft_strncmp(token->data, "unset", 6) == 0)
		ft_unset(data, token->child);
}
