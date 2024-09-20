/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:00:14 by icseri            #+#    #+#             */
/*   Updated: 2024/09/19 15:26:52 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*ft_getenv(t_var *data, char *var_name)
{
	int	i;
	int	len;

	i = 0;
	if (var_name && *var_name)
	{
		len = ft_strlen(var_name);
		while (data->env[i])
		{
			if (ft_strncmp(data->env[i], var_name, len) == 0
				&& data->env[i][len] == '=')
				return (data->env[i] + len + 1);
			i++;
		}
	}
	return (NULL);
}

void	init_env(t_var *data)
{
	extern char	**environ;
	int			size;
	int			i;

	size = 0;
	while (environ && environ[size])
		size++;
	data->env = malloc(sizeof(char *) * (size + 1));
	if (!data->env)
		safe_exit(data, MALLOC_FAIL);
	i = 0;
	while (i < size)
	{
		data->env[i] = ft_strdup(environ[i]);
		if (!data->env[i++])
			safe_exit(data, MALLOC_FAIL);
	}
	data->env[i] = NULL;
}

char	*ft_strjoin2(char *str1, char *str2, char *delimiter)
{
	char	*tmp;
	char	*new_str;

	tmp = ft_strjoin(str1, delimiter);
	if (!tmp)
		return (NULL);
	new_str = ft_strjoin(tmp, str2);
	free(tmp);
	if (!new_str)
		return (NULL);
	return (new_str);
}

bool	replace_var(t_var *data, char *key, char *content)
{
	int	i;

	i = 0;
	while (data->env && data->env[i])
	{
		if (ft_strncmp(data->env[i], key, ft_strlen(key) + 1) == 0
			&& data->env[i][ft_strlen(key)] == '=')
		{
			free(data->env[i]);
			data->env[i] = ft_strjoin2(key, content, "=");
			if (!data->env[i])
				data->exit_code = MALLOC_FAIL;
			return (true);
		}
		i++;
	}
	return (false);
}

int	add_var_to_env(t_var *data, char *line)
{
	int		size;
	char	**new_env;

	size = 0;
	while (data->env && data->env[size])
		size++;
	new_env = ft_calloc(sizeof(char *), size + 2);
	if (!new_env)
		return (MALLOC_FAIL);
	new_env[size] = ft_strdup(line);
	if (!new_env[size])
	{
		free_array(new_env);
		return (MALLOC_FAIL);
	}
	while (--size >= 0)
		new_env[size] = data->env[size];
	free(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
}
