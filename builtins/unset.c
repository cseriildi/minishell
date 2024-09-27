/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:53:04 by icseri            #+#    #+#             */
/*   Updated: 2024/09/27 13:45:27 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	shift_env(t_var *data, int index)
{
	free(data->env[index]);
	while (data->env[index + 1])
	{
		data->env[index] = data->env[index + 1];
		index++;
	}
	data->env[index] = NULL;
}

void	ft_unset(t_var *data)
{
	int	i;
	int	j;
	int	len;

	data->exit_code = EXIT_SUCCESS;
	i = 0;
	while (data->cmd_list[++i])
	{
		len = ft_strlen(data->cmd_list[i]);
		j = -1;
		while (len != 0 && data->env && data->env[++j])
		{
			if (ft_strncmp(data->env[j], data->cmd_list[i], len) == 0
				&& data->env[j][len] == '=')
			{
				shift_env(data, j);
				break ;
			}
		}
	}
}
