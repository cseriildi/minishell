/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:38:19 by pvass             #+#    #+#             */
/*   Updated: 2024/09/30 15:49:28 by pvass            ###   ########.fr       */
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
		data->exit_code = MALLOC_FAIL;
	}
	i = -1;
	while (++i < count)
		indices[i] = i;
	bubble_sort_with_context(indices, count, data->env);
	i = -1;
	while (++i < count)
		print_export(data->env[indices[i]]);
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
	char	*var_name;

	var_name = get_word(line, "=");
	if (!var_name)
		p_error_exp(data);
	i = 0;
	if (ft_isdigit(line[i]))
	{
		data->exit_code = ERROR_MISUSE;
		return (print_error(2, "export: not an identifier: ", var_name), false);
	}
	while (line[i] && line[i] != '=')
	{
		if (!ft_isalnum(line[i]) && line[i] != '_')
		{
			print_error(2, "export: not valid in this context ", var_name);
			data->exit_code = ERROR_MISUSE;
			return (false);
		}
		i++;
	}
	if (line[i] != '=')
		return (false);
	return (true);
}

void	ex_util2(t_var *data)
{
	print_error(2, "export: ", strerror(errno));
	data->exit_code = MALLOC_FAIL;
}