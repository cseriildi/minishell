/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:49 by icseri            #+#    #+#             */
/*   Updated: 2024/10/22 08:35:36 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_env(t_var *data)
{
	int	i;

	i = -1;
	while (data->env && data->env[++i])
	{
		if (ft_strchr(data->env[i], '='))
			ft_putendl_fd(data->env[i], data->fd_to_write);
	}
	data->exit_code = EXIT_SUCCESS;
}
