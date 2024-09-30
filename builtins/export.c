/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:56 by icseri            #+#    #+#             */
/*   Updated: 2024/09/30 15:52:31 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	compare_indices(int a, int b, char **env)
{
	int	i;

	i = 0;
	while (env[a][i] && env[b][i] && env[a][i] == env[b][i])
		i++;
	return (env[a][i] - env[b][i]);
}

void	bubble_sort_with_context(int *arr, int n, char **env)
{
	int	i;
	int	j;
	int	temp;

	i = -1;
	while (++i < n - 1)
	{
		j = -1;
		while (++j < n - i - 1)
		{
			if (compare_indices(arr[j], arr[j + 1], env) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void	print_export(char *line)
{
	int	i;

	i = 0;
	ft_putstr_fd("declare -x ", 1);
	while (line[i] && line[i] != '=')
		ft_putchar_fd(line[i++], 1);
	printf("=\"%s\"\n", line + i + 1);
}

void	ex_util(t_var *data, char **line)
{
	print_error(2, "export: ", strerror(errno));
	data->exit_code = MALLOC_FAIL;
	free_array(&line);
}

void	ft_export(t_var *data)
{
	char	**line;
	int		i;

	data->exit_code = EXIT_SUCCESS;
	if (data->cmd_list[1] == NULL)
		no_arg_export(data);
	i = 0;
	while (data->cmd_list[++i])
	{
		if (is_valid_var(data, data->cmd_list[i]) == true)
		{
			line = ft_split(data->cmd_list[1], '=');
			if (!line)
			{
				ex_util(data, line);
				continue ;
			}
			if (replace_var(data, line[0], line[1]) == false)
			{
				if (add_var_to_env(data, data->cmd_list[i]) == MALLOC_FAIL)
					ex_util2(data);
			}
			free_array(&line);
		}
	}
}
