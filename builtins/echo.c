/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/09/27 13:43:11 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_echo(t_var *data)
{
	int		i;
	bool	n_flag;

	i = 0;
	n_flag = false;
	if (data->cmd_list[1] && ft_strncmp(data->cmd_list[1], "-n", 3) == 0)
	{
		n_flag = true;
		i++;
	}
	while (data->cmd_list[++i])
	{
		printf("%s", data->cmd_list[i]);
		if (data->cmd_list[i + 1] != NULL)
			printf(" ");
	}
	if (n_flag == false)
		printf("\n");
	data->exit_code = EXIT_SUCCESS;
}
