/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:56 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 20:22:48 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	add_var_to_env(t_var *data, char *line)
{
	int		size;
	char	**new_env;
	int		i;

	size = 0;
	while (data->env && data->env[size])
		size++;
	new_env = ft_calloc(sizeof(char *), size + 2);
	if (!new_env)
		return (MALLOC_FAIL);
	i = -1;
	while (++i < size)
		new_env[i] = data->env[i];
	new_env[size] = ft_strdup(line);
	if (!new_env[size])
		return (free_array(new_env), MALLOC_FAIL);
	free(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
}
bool	is_valid_var(t_var *data, char *line)
{
	int		i;
	char	*var_name;

	var_name = get_word(line, "=");
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

void	ft_export(t_var *data, t_ast *token)
{
	char	**line;
	
	if (!token->data)
	{
		//print all env in ascii order
		return ;
	}
	if (is_valid_var(data, token->data) == false)
		return ;
	line = ft_split(token->data, '=');
	if (!line)
	{
		print_error(MALLOC_FAIL, NULL);
		data->exit_code = MALLOC_FAIL;
		return ;
	}
	if (replace_var(data, line[0], line[1]) == false)
	{
		if (add_var_to_env(data, token->data) == MALLOC_FAIL)
		{
			data->exit_code = MALLOC_FAIL;
			free_array(line);
			return ;
		}
	}
	free_array(line);
	data->exit_code = EXIT_SUCCESS;
}
