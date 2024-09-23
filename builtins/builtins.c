/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:44:24 by icseri            #+#    #+#             */
/*   Updated: 2024/09/17 15:59:14 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

bool	exec_builtin(t_var *data, char **cmd_list)
{
	if (ft_strncmp(cmd_list[0], "cd", 3) == 0)
		return (ft_cd(data, cmd_list), true);
	if (ft_strncmp(cmd_list[0], "echo", 5) == 0)
		return (ft_echo(data, cmd_list), true);
	if (ft_strncmp(cmd_list[0], "env", 4) == 0)
		return (ft_env(data), true);
	if (ft_strncmp(cmd_list[0], "exit", 5) == 0)
		return (ft_exit(data, cmd_list), true);
	if (ft_strncmp(cmd_list[0], "export", 7) == 0)
		return (ft_export(data, cmd_list), true);
	if (ft_strncmp(cmd_list[0], "pwd", 3) == 0)
		return (ft_pwd(data), true);
	if (ft_strncmp(cmd_list[0], "unset", 5) == 0)
		return (ft_unset(data, cmd_list), true);
	return (false);
}
