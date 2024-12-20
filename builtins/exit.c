/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:52 by icseri            #+#    #+#             */
/*   Updated: 2024/10/23 12:09:26 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_exit(t_var *data)
{
	if (isatty(STDIN_FILENO))
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
		data->exit_code = EXIT_FAILURE;
		return ;
	}
	safe_exit(data, data->exit_code);
}
