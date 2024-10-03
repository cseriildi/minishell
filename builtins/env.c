/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:49 by icseri            #+#    #+#             */
/*   Updated: 2024/10/03 13:05:17 by icseri           ###   ########.fr       */
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
			printf("%s\n", data->env[i]);
	}
	data->exit_code = EXIT_SUCCESS;
}
