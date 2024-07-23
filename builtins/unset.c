/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:53:04 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 18:21:35 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_unset(t_var *data, t_ast *token)
{
	int	i;
	int	len;

	len = 0;
	if (token->data)
		len = ft_strlen(token->data);
	i = 0;
	while (data->env && data->env[i])
	{
		if (ft_strncmp(data->env[i], token->data, len + 1) == 0
			&& data->env[i][len] == '=')
		{
			free(data->env[i]);
			data->env[i] = data->env[i + 1];
			break ;
		}
		i++;
	}
	data->exit_code = EXIT_SUCCESS;
}
