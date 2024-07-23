/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:44:24 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 15:37:48 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

bool	builtin(t_var *data, t_ast *token)
{
	if (ft_strncmp(token->data, "cd", 3) == 0)
		return (ft_cd(data, token->child), true);
	else if (ft_strncmp(token->data, "echo", 5) == 0)
		return (ft_echo(data, token->child), true);
	else if (ft_strncmp(token->data, "env", 4) == 0)
		return (ft_env(data), true);
	else if (ft_strncmp(token->data, "exit", 5) == 0)
		return (ft_exit(data, token->child), true);
	else if (ft_strncmp(token->data, "export", 7) == 0)
		return (ft_export(data, token->child), true);
	else if (ft_strncmp(token->data, "pwd", 4) == 0)
		return (ft_pwd(data), true);
	else if (ft_strncmp(token->data, "unset", 6) == 0)
		return (ft_unset(data, token->child), true);
	return (false);
}
