/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:52 by icseri            #+#    #+#             */
/*   Updated: 2024/09/17 17:20:54 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	get_exit_code(char *str)
{
	int	i;

	i = 0;
	if (*str == '-' || *str == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			print_error(3, "minishell: exit: ", str, ": numeric argument required");
			return (ERROR_MISUSE);
		}
		i++;
	}
	//I have to handle long overflow, and the exit code should be % 256
	return (ft_atoi(str));
}


void	ft_exit(t_var *data, char **cmd_list)
{
	printf("exit\n");
	if (cmd_list[1] && cmd_list[2])
	{
		print_error(3, "minishell: exit: too many arguments");
		data->exit_code = ERROR_MISUSE;
		return ;
	}
	else if (cmd_list[1])
		data->exit_code = get_exit_code(cmd_list[1]);
	safe_exit(data, data->exit_code);
}
