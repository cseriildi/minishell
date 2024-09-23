/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/09/17 16:55:35 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_echo(t_var *data, char **cmd_list)
{
	int		i;
	bool	n_flag;

	i = 1;
	n_flag = false;
	if (cmd_list[1] && ft_strncmp(cmd_list[1], "-n", 3) == 0)
	{
		n_flag = true;
		i++;
	}
	while (cmd_list[i])
		printf("%s", cmd_list[i++]);
	if (n_flag == false)
		printf("\n");
	data->exit_code = EXIT_SUCCESS;
}
