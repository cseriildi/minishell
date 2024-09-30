/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:52 by icseri            #+#    #+#             */
/*   Updated: 2024/09/30 17:01:26 by icseri           ###   ########.fr       */
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
			print_error(3, "minishell: exit: ", str,
				": numeric argument required");
			return (ERROR_MISUSE);
		}
		i++;
	}
	//I have to handle long overflow, and the exit code should be % 256
	return (ft_atoi(str));
}

void	ft_exit(t_var *data)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	if (data->cmd_list[1] && data->cmd_list[2])
	{
		print_error(3, "minishell: exit: too many arguments");
		data->exit_code = ERROR_MISUSE;
		return ;
	}
	else if (data->cmd_list[1])
		data->exit_code = get_exit_code(data->cmd_list[1]);
	safe_exit(data, data->exit_code);
}
