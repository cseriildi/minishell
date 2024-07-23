/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:52 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 19:40:45 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	get_exit_code(char *str)
{
	int	i;

	i = 0;
	if (*str == '-' || *str == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			print_error(3, "minishell: exit: ", str, ": numeric argument required");
			return (ERROR_MISUSE);
		}
		i++;
	}
	return (ft_atoi(str));
}


void	ft_exit(t_var *data, t_ast *token)
{
	printf("exit\n");
	if (token->data)
		data->exit_code = get_exit_code(token->data);
	if (--data->subshell_level == 0)
		safe_exit(data, data->exit_code);
	exit(data->exit_code);
}
