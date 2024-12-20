/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:53:01 by icseri            #+#    #+#             */
/*   Updated: 2024/10/22 08:35:14 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_pwd(t_var *data)
{
	ft_putendl_fd(data->pwd, data->fd_to_write);
	data->exit_code = EXIT_SUCCESS;
}
