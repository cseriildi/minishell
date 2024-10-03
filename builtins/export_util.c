/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:38:19 by pvass             #+#    #+#             */
/*   Updated: 2024/10/03 13:06:07 by icseri           ###   ########.fr       */
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
	{
		print_error(2, "export: ", strerror(errno));
		safe_exit(data, MALLOC_FAIL);
	}
	i = -1;
	while (++i < count)
		indices[i] = i;
	bubble_sort_with_context(indices, count, data->env);
	i = -1;
	while (++i < count)
	{
		if (ft_strncmp(data->env[indices[i]], "_=", 2))
			print_export(data->env[indices[i]]);
	}
	free(indices);
}

void	p_error_exp(t_var *data)
{
	print_error(2, "minishell: malloc failed: ", strerror(errno));
	safe_exit(data, MALLOC_FAIL);
}

bool	is_valid_var(t_var *data, char *line)
{
	int		i;

	i = 0;
	if (line[i] && line[i] == '=')
	{
		data->exit_code = 1;
		return (print_error(3, "minishell: export: `", line,"': not a valid identifier"), false);
	}
	while (line[i] && line[i] != '=')
	{
		if ((i == 0 && (!ft_isalpha(line[i]) && line[i] != '_'))
			|| (!ft_isalnum(line[i]) && line[i] != '_'))
		{
			data->exit_code = 1;
			return (print_error(3, "minishell: export: `", line,"': not a valid identifier"), false);
		}
		i++;
	}
	return (true);
}

void	ex_util2(t_var *data)
{
	print_error(2, "export: ", strerror(errno));
	data->exit_code = MALLOC_FAIL;
}
