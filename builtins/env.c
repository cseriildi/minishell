/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:49 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 19:10:11 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_env(t_var *data)
{
	int	i;

	i = -1;
	while (data->env && data->env[++i])
		printf("%s\n", data->env[i]);
	data->exit_code = EXIT_SUCCESS;
}
