/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/10/21 13:50:30 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_echo(t_var *data)
{
	int		i;
	bool	n_flag;

	n_flag = is_n_flag(data->cmd_list[1]);
	i = n_flag + 1;
	while (data->cmd_list[i] && is_n_flag(data->cmd_list[i]))
		i++;
	while (data->cmd_list[i])
	{
		ft_putstr_fd(data->cmd_list[i], data->fd_to_write);
		if (data->cmd_list[i + 1] != NULL)
			ft_putchar_fd(' ', data->fd_to_write);
		i++;
	}
	if (n_flag == false)
		ft_putchar_fd('\n', data->fd_to_write);
	data->exit_code = EXIT_SUCCESS;
}

bool is_n_flag(char *str)
{
	int	i;

	if (str == NULL || !str[0] || str[0] != '-' || !str[1])
		return (false);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}