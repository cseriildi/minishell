/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 10:36:32 by pvass             #+#    #+#             */
/*   Updated: 2024/10/25 12:13:28 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_in_out_app(t_exec *exec)
{
	if (exec->type == RED_IN || exec->type == RED_OUT || exec->type == APPEND)
		return (1);
	else
		return (0);
}

bool	good_redir_path(char *redir, t_var *data)
{
	char	*new;
	int		i;
	int		count;
	bool	res;

	count = 0;
	i = 0;
	while (redir[i] != '\0')
	{
		if (redir[i] == '/')
			count = i;
		i++;
	}
	if (count == 0)
		return (1);
	new = ft_substr(redir, 0, count);
	if (new == NULL)
		malloc_failed(data);
	res = is_directory(new);
	free(new);
	return (res);
}

int	count_exec(t_exec *exec)
{
	t_exec	*temp;
	int		i;

	i = 0;
	temp = exec;
	while (temp != NULL)
	{
		i++;
		temp = temp->down;
	}
	return (i);
}

bool	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (false);
	return (S_ISDIR(path_stat.st_mode));
}

char	**get_paths(t_var *data)
{
	int		i;
	char	**path;

	if (*ft_getenv(data, "PATH", true) == '\0')
		return (NULL);
	i = 0;
	while (data->env[i] != NULL)
	{
		if (ft_strncmp(data->env[i], "PATH=", 5) == 0)
		{
			path = ft_split(data->env[i] + 5, ':');
			if (!path)
				malloc_failed(data);
			return (path);
		}
		i++;
	}
	return (NULL);
}
