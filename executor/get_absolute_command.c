/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_absolute_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 10:46:48 by pvass             #+#    #+#             */
/*   Updated: 2024/10/25 12:13:58 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*check_dot_and_slash(t_var *data, char *cmd)
{
	char	*path_cmd;

	path_cmd = NULL;
	if (*cmd == '/')
	{
		path_cmd = ft_strdup(cmd);
		if (path_cmd == NULL)
			malloc_failed(data);
		return (path_cmd);
	}
	if (*cmd == '.')
	{
		if (cmd[1] == '.' || cmd[1] == '/' || cmd[1] == '\0')
		{
			path_cmd = ft_strdup(cmd);
			if (path_cmd == NULL)
				malloc_failed(data);
			return (path_cmd);
		}
	}
	return (path_cmd);
}

char	*path_handle(t_var *data, char *cmd, char **path)
{
	char	*path_cmd;

	path_cmd = NULL;
	if (!path)
	{
		path_cmd = ft_strdup(cmd);
		if (path_cmd == NULL)
			malloc_failed(data);
		return (path_cmd);
	}
	return (path_cmd);
}

void	freepath_and_exit(t_var *data, char **path)
{
	free_array(&path);
	malloc_failed(data);
}

char	*get_abs_cmd(t_var *data, char *cmd)
{
	int		i;
	char	**path;
	char	*path_cmd;

	if (!*cmd)
		return (NULL);
	path_cmd = check_dot_and_slash(data, cmd);
	if (path_cmd != NULL)
		return (path_cmd);
	path = get_paths(data);
	path_cmd = path_handle(data, cmd, path);
	if (path_cmd != NULL)
		return (path_cmd);
	i = -1;
	while (path[++i] != NULL)
	{
		path_cmd = ft_strjoin2(path[i], cmd, "/");
		if (!path_cmd)
			freepath_and_exit(data, path);
		if (access(path_cmd, F_OK) == 0)
			return (free_array(&path), path_cmd);
		ft_free(&path_cmd);
	}
	free_array(&path);
	return (path_cmd);
}
