/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:56 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 16:01:45 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_export(t_var *data, t_ast *token)
{
	t_env	*new_var;
	char	**line;

	line = ft_split(token->data, '=');
	if (!line)
		safe_exit(data, MALLOC_FAIL);
	del_var(data->env, line[0]);
	if (line[1])
		new_var = create_new_var(line[0], line[1]);
	else
		new_var = create_new_var(line[0], "");
	free_array(line);
	if (!new_var)
		safe_exit(data, MALLOC_FAIL);
	add_var_to_back(data->env, new_var);
	exit(EXIT_SUCCESS);
}
