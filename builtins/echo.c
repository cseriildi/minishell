/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/10/02 16:55:35 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_echo(t_var *data)
{
	int		i;
	bool	n_flag;

	n_flag = is_n_flag(data->cmd_list[1]);
	i = n_flag + 1;
	while (data->cmd_list[i] && is_n_flag(data->cmd_list[i]))
		i++;
	while (data->cmd_list[i])
	{
		printf("%s", data->cmd_list[i]);
		if (data->cmd_list[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (n_flag == false)
		printf("\n");
	data->exit_code = EXIT_SUCCESS;
}

bool is_n_flag(char *str)
{
	int	i;

	if (str == NULL || !str[0] || str[0] != '-' || !str[1])
		return (false);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}