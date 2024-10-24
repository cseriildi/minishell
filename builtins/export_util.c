/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:38:19 by pvass             #+#    #+#             */
/*   Updated: 2024/10/24 18:43:33 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	no_arg_export(t_var *data)
{
	int	count;
	int	*indices;
	int	i;

	count = 0;
	while (data->env[count])
		count++;
	indices = malloc(count * sizeof(int));
	if (indices == NULL)
		malloc_failed(data);
	i = -1;
	while (++i < count)
		indices[i] = i;
	bubble_sort_with_context(indices, count, data->env);
	i = -1;
	while (++i < count)
	{
		if (ft_strncmp(data->env[indices[i]], "_=", 2))
			print_export(data, data->env[indices[i]]);
	}
	free(indices);
}

bool	is_valid_var(t_var *data, char *line)
{
	int		i;

	i = 0;
	if (line[i] && line[i] == '=')
	{
		data->exit_code = 1;
		print_error(3, "minishell: export: `",
			line, "': not a valid identifier");
		return (false);
	}
	while (line[i] && line[i] != '=')
	{
		if ((i == 0 && (!ft_isalpha(line[i]) && line[i] != '_'))
			|| (!ft_isalnum(line[i]) && line[i] != '_'))
		{
			data->exit_code = 1;
			print_error(3, "minishell: export: `",
				line, "': not a valid identifier");
			return (false);
		}
		i++;
	}
	return (true);
}
