/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:44:24 by icseri            #+#    #+#             */
/*   Updated: 2024/10/04 04:11:27 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

bool	exec_builtin(t_var *data)
{
	if (data->cmd_list[0] == NULL)
		return (true);
	if (ft_strncmp(data->cmd_list[0], "cd", 3) == 0)
		ft_cd(data);
	else if (ft_strncmp(data->cmd_list[0], "echo", 5) == 0)
		ft_echo(data);
	else if (ft_strncmp(data->cmd_list[0], "env", 4) == 0)
		ft_env(data);
	else if (ft_strncmp(data->cmd_list[0], "exit", 5) == 0)
		ft_exit(data);
	else if (ft_strncmp(data->cmd_list[0], "export", 7) == 0)
		ft_export(data);
	else if (ft_strncmp(data->cmd_list[0], "pwd", 3) == 0)
		ft_pwd(data);
	else if (ft_strncmp(data->cmd_list[0], "unset", 5) == 0)
		ft_unset(data);
	if (is_builtin(data->cmd_list[0]) == true)
		return (true);
	return (false);
}

bool	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) && ft_strncmp(cmd, "cd", 3)
		&& ft_strncmp(cmd, "pwd", 4) && ft_strncmp(cmd, "export", 7)
		&& ft_strncmp(cmd, "unset", 5) && ft_strncmp(cmd, "env", 4)
		&& ft_strncmp(cmd, "exit", 5))
		return (false);
	return (true);
}
