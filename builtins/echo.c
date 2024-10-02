/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:42 by icseri            #+#    #+#             */
/*   Updated: 2024/10/02 16:04:18 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_echo(t_var *data)
{
	int		i;
	bool	n_flag;

	n_flag = is_n_flag(data->cmd_list[1]);
	i = n_flag;
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

bool is_n_flag(char *str)
{
	int	i;

	if (str == NULL || str[0] != '-')
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