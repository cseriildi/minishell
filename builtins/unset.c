/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:53:04 by icseri            #+#    #+#             */
/*   Updated: 2024/09/17 17:26:51 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_unset(t_var *data, char **cmd_list)
{
	int	i;
	int	len;

	data->exit_code = EXIT_SUCCESS;
	if (cmd_list[1] == NULL)
		return ;
	len = 0;
	//I have to loop through the cmd_list and remove all not just the first
	if (cmd_list[1])
		len = ft_strlen(cmd_list[1]);
	i = 0;
	while (data->env && data->env[i])
	{
		if (ft_strncmp(data->env[i], cmd_list[1], len) == 0
			&& data->env[i][len] == '=')
		{
			free(data->env[i]);
			data->env[i] = data->env[i + 1];
			break ;
		}
		i++;
	}
}
