/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:56 by icseri            #+#    #+#             */
/*   Updated: 2024/09/30 10:51:04 by cseriildii       ###   ########.fr       */
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

bool	is_valid_var(t_var *data, char *line)
{
	int		i;
	char	*var_name;

	var_name = get_word(line, "=");
	if (!var_name)
	{
		print_error(2, "minishell: malloc failed: ", strerror(errno));
		safe_exit(data, MALLOC_FAIL);
	}
	i = 0;
	if (ft_isdigit(line[i]))
	{
		print_error(2, "export: not an identifier: ", var_name);
		data->exit_code = ERROR_MISUSE;
		return (false);
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
				print_error(2, "export: ", strerror(errno));
				data->exit_code = MALLOC_FAIL;
				free_array(&line);
				continue ;
			}
			if (replace_var(data, line[0], line[1]) == false)
			{
				if (add_var_to_env(data, data->cmd_list[i]) == MALLOC_FAIL)
				{
					print_error(2, "export: ", strerror(errno));
					data->exit_code = MALLOC_FAIL;
				}
			}
			free_array(&line);
		}
	}
}
