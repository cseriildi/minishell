/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:08 by icseri            #+#    #+#             */
/*   Updated: 2024/11/06 13:56:11 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <string.h>

char	*safe_getcwd(t_var *data)
{
	char	*temp;

	temp = getcwd(NULL, 0);
	if (temp != NULL)
		return (temp);
	if (errno == EACCES || errno == ENOENT)
	{
		print_error(3, "cd: error retrieving current directory: ",
			"getcwd: cannot access parent directories: ",
			"No such file or directory");
		return (NULL);
	}
	else
		return(malloc_failed(data), NULL);
}

char	*get_dest(t_var *data)
{
	char	*dest;
	char	*tmp;

	dest = data->cmd_list[1];
	if (!data->cmd_list[1])
	{
		dest = ft_getenv(data, "HOME", false);
		if (dest == NULL)
			return (print_error(1, "minishell: cd: HOME not set"), NULL);
	}
	else if (data->cmd_list[2])
		return (print_error(1, "minishell: cd: too many arguments"), NULL);
	else if (ft_strncmp(data->cmd_list[1], "-", 2) == 0)
	{
		dest = ft_getenv(data, "OLDPWD", false);
		if (dest == NULL)
			return (print_error(1, "minishell: cd: OLDPWD not set"), NULL);
		ft_putendl_fd(dest, data->fd_to_write);
	}
	else if (ft_strncmp(data->cmd_list[1], ".", 2) == 0
		|| ft_strncmp(data->cmd_list[1], "./", 3) == 0)
	{
		tmp = safe_getcwd(data);
		if (!tmp)
			return (NULL);
		free(tmp);
	}
	return (dest);
}

int	cd_in_env(t_var *data)
{
	char	*tmp;

	tmp = ft_strjoin("OLDPWD=", ft_getenv(data, "PWD", true));
	if (!tmp)
		return (MALLOC_FAIL);
	if (replace_var(data, tmp) == false)
	{
		if (add_var_to_env(data, tmp) == MALLOC_FAIL)
			return (free(tmp), MALLOC_FAIL);
	}
	free(tmp);
	ft_free(&data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		return (print_error(2, "cd: ", strerror(errno)), EXIT_SUCCESS);
	tmp = ft_strjoin("PWD=", data->pwd);
	if (!tmp)
		return (MALLOC_FAIL);
	if (replace_var(data, tmp) == false)
	{
		if (add_var_to_env(data, tmp) == MALLOC_FAIL)
			return (free(tmp), MALLOC_FAIL);
	}
	free(tmp);
	return (EXIT_SUCCESS);
}

int retry_chdir(t_var *data, char *dest)
{
	int		len;
	char	*corrected_dest;

	if (ft_strncmp("..", dest, 3) == 0 || ft_strncmp("../", dest, 4) == 0)
	{
		len = ft_strlen(data->pwd) - 1;
		if (data->pwd[len] == '/')
			len--;
		while (--len > 0 && data->pwd[len] != '/')
			continue ;
		corrected_dest = ft_substr(data->pwd, 0, len);
		if (!corrected_dest)
			malloc_failed(data);
		return (chdir(corrected_dest));
	}
	return (-1);
}

void	ft_cd(t_var *data)
{
	char	*dest;

	data->exit_code = EXIT_SUCCESS;
	dest = get_dest(data);
	if (!dest)
	{
		data->exit_code = EXIT_FAILURE;
		return ;
	}
	if (chdir(dest) == -1 && ((errno == EACCES && retry_chdir(data, dest) == -1)
		|| errno != EACCES))
	{
		print_error(4, "minishell: cd: ", dest, ": ", strerror(errno));
		data->exit_code = EXIT_FAILURE;
		return ;
	}
	if (cd_in_env(data) == MALLOC_FAIL)
		malloc_failed(data);
}
