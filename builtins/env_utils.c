/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:00:14 by icseri            #+#    #+#             */
/*   Updated: 2024/10/24 22:36:45 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*ft_getenv(t_var *data, char *var_name, bool is_safe)
{
	int	i;
	int	len;

	i = 0;
	if (var_name && *var_name)
	{
		len = ft_strlen(var_name);
		while (data->env && data->env[i])
		{
			if (ft_strncmp(data->env[i], var_name, len) == 0
				&& data->env[i][len] == '=')
				return (data->env[i] + len + 1);
			i++;
		}
	}
	if (is_safe)
		return ("");
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
		malloc_failed(data);
	i = 0;
	while (i < size)
	{
		data->env[i] = ft_strdup(environ[i]);
		if (!data->env[i++])
			malloc_failed(data);
	}
	data->env[i] = NULL;
}

bool	find_and_replace(t_var *data, char *line, int len)
{
	int		i;

	i = -1;
	while (data->env && data->env[++i])
	{
		if (!ft_strncmp(data->env[i], line, len)
			&& (!data->env[i][len] || data->env[i][len] == '='))
		{
			ft_free(&data->env[i]);
			data->env[i] = ft_strdup(line);
			if (!data->env[i])
				malloc_failed(data);
			return (true);
		}
	}
	return (false);
}

bool	replace_var(t_var *data, char *line)
{
	int		len;
	char	*key;
	bool	exists;

	key = get_word(line, "=");
	if (!key)
		malloc_failed(data);
	exists = (ft_getenv(data, key, false) != NULL);
	len = ft_strlen(key);
	free(key);
	if (exists == false || ft_strchr(line, '=') == NULL)
		return (exists);
	return (find_and_replace(data, line, len));
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
		free_array(&new_env);
		return (MALLOC_FAIL);
	}
	while (--size >= 0)
		new_env[size] = data->env[size];
	free(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
}
