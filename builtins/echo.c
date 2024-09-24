/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/09/24 15:03:59 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_echo(t_var *data)
{
	int		i;
	bool	n_flag;

	i = 1;
	n_flag = false;
	if (data->cmd_list[1] && ft_strncmp(data->cmd_list[1], "-n", 3) == 0)
	{
		n_flag = true;
		i++;
	}
	while (data->cmd_list[i])
		printf("%s", data->cmd_list[i++]);
	if (n_flag == false)
		printf("\n");
	data->exit_code = EXIT_SUCCESS;
}
