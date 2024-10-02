/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:52 by icseri            #+#    #+#             */
/*   Updated: 2024/10/02 17:47:31 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	set_exit_code(t_var *data)
{
	int	i;

	i = 0;
	if (*data->cmd_list[1] == '-' || *data->cmd_list[1] == '+')
		i++;
	while (data->cmd_list[1][i])
	{
		if (!ft_isdigit(data->cmd_list[1][i]))
		{
			print_error(3, "minishell: exit: ", data->cmd_list[1],
				": numeric argument required");
			data->exit_code = ERROR_MISUSE;
			return ;
		}
		i++;
	}
	data->exit_code = ft_atoi(data->cmd_list[1]) % 256;
	if (data->cmd_list[2])
	{
		print_error(1, "minishell: exit: too many arguments");
		data->exit_code = ERROR_MISUSE;
		return ;
	}
	
	//I have to handle long overflow, and the exit code should be % 256
}

void	ft_exit(t_var *data)
{
	int		i;
	
	i = 0;
	ft_putendl_fd("exit", STDERR_FILENO);
	if (!data->cmd_list[1])
		safe_exit(data, data->exit_code);
	data->exit_code = ft_atoll(data->cmd_list[1]) % 256;
	if (is_ll_overflow(data, data->cmd_list[1]) == true)
	{
		print_error(3, "minishell: exit: ", data->cmd_list[1],
			": numeric argument required");
		safe_exit(data, ERROR_MISUSE);
	}
	if (data->cmd_list[2])
	{
		print_error(1, "minishell: exit: too many arguments");
		data->exit_code = ERROR_MISUSE;
		return ;
	}
	safe_exit(data, data->exit_code);
}
