/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:08 by icseri            #+#    #+#             */
/*   Updated: 2024/10/22 08:35:50 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	cd_in_env(t_var *data, char *pwd)
{
	char	*tmp;

	tmp = ft_strjoin("PWD=", pwd);
	if (!tmp)
		return (MALLOC_FAIL);
	if (replace_var(data, tmp) == false)
	{
		if (add_var_to_env(data, tmp) == MALLOC_FAIL)
			return (free(tmp), MALLOC_FAIL);
	}
	free(tmp);
	tmp = ft_strjoin("OLDPWD=", data->pwd);
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

void	ft_cd(t_var *data)
{
	char	*pwd;
	char	*dest;

	data->exit_code = EXIT_SUCCESS;
	dest = data->cmd_list[1];
	if (dest == NULL)
	{
		dest = ft_getenv(data, "HOME");
		if (dest == NULL)
		{
			print_error(1, "minishell: cd: HOME not set");
			data->exit_code = EXIT_FAILURE;
			return ;
		}
	}
	if (data->cmd_list[1] && data->cmd_list[2])
	{
		print_error(1, "minishell: cd: too many arguments");
		data->exit_code = EXIT_FAILURE;
		return ;
	}
	if (data->cmd_list[1] && ft_strncmp(data->cmd_list[1], "-", 2) == 0)
	{
		dest = ft_getenv(data, "OLDPWD");
		if (dest == NULL)
		{
			print_error(1, "minishell: cd: OLDPWD not set");
			data->exit_code = EXIT_FAILURE;
			return ;
		}
		else 
			ft_putendl_fd(dest, data->fd_to_write);
	}
	if (chdir(dest) == -1)
	{
		print_error(4, "minishell: cd: ", dest, ": ", strerror(errno));
		data->exit_code = EXIT_FAILURE;
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (cd_in_env(data, pwd) == MALLOC_FAIL)
	{
		print_error(4, "minishell: cd: ", dest, ": ", strerror(errno));
		data->exit_code = MALLOC_FAIL;
	}
	ft_free(&data->pwd);
	data->pwd = pwd;
}
