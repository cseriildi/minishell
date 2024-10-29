/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:08 by icseri            #+#    #+#             */
/*   Updated: 2024/10/29 15:09:25 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*get_dest(t_var *data)
{
	char	*dest;

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
	else if ((ft_strncmp(data->cmd_list[1], ".", 2) == 0
			|| ft_strncmp(data->cmd_list[1], "./", 2) == 0)
		&& getcwd(NULL, 0) == NULL)
		return (print_error(3, "cd: error retrieving current directory: ",
				"getcwd: cannot access parent directories: ",
				"No such file or directory"), NULL);
	return (dest);
}

int	cd_in_env(t_var *data)
{
	char	*tmp;

	tmp = ft_strjoin("OLDPWD=", data->pwd);
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

char	*get_corrected_dest(t_var *data, char *dest)
{
	int		len;
	char	*corrected_dest;

	len = ft_strlen(data->pwd) - 1;
	if (data->pwd[len] == '/')
		len--;
	while (--len && data->pwd[len] != '/')
		continue ;
	if (ft_strncmp("..", dest, 3) == 0 || ft_strncmp("../", dest, 4) == 0)
		corrected_dest = ft_substr(data->pwd, 0, len);
	else
		corrected_dest = ft_strdup(dest);
	return (corrected_dest);
}

void	ft_cd(t_var *data)
{
	char	*dest;
	char	*corrected_dest;

	data->exit_code = EXIT_SUCCESS;
	dest = get_dest(data);
	if (!dest)
	{
		data->exit_code = EXIT_FAILURE;
		return ;
	}
	corrected_dest = get_corrected_dest(data, dest);
	if (!corrected_dest)
		malloc_failed(data);
	if (chdir(corrected_dest) == -1)
	{
		print_error(4, "minishell: cd: ", dest, ": ", strerror(errno));
		data->exit_code = EXIT_FAILURE;
		ft_free(&corrected_dest);
		return ;
	}
	ft_free(&corrected_dest);
	if (cd_in_env(data) == MALLOC_FAIL)
		malloc_failed(data);
}
