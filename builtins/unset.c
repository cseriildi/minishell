/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:53:04 by icseri            #+#    #+#             */
/*   Updated: 2024/09/24 15:34:32 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_unset(t_var *data)
{
	int	i;
	int	j;
	int	len;

	data->exit_code = EXIT_SUCCESS;
	i = 0;
	while (data->cmd_list[++i] == NULL)
	{
		len = 0;
		if (data->cmd_list[i])
			len = ft_strlen(data->cmd_list[i]);
		j = -1;
		while (data->env && data->env[++j])
		{
			if (ft_strncmp(data->env[j], data->cmd_list[i], len) == 0
				&& data->env[j][len] == '=')
			{
				free(data->env[j]);
				data->env[j] = data->env[j + 1];
				break ;
			}
		}
	}
}
