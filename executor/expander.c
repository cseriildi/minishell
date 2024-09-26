/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:24:05 by icseri            #+#    #+#             */
/*   Updated: 2024/09/26 13:32:44 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*expander(t_var *data, char *str)
{
	char	*expanded;

	expanded = ft_strdup(str);
	if (!expanded)
	{
		data->exit_code = MALLOC_FAIL;
		return (NULL);
	}
	//implement
	return (expanded);
}
