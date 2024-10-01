/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:08 by icseri            #+#    #+#             */
/*   Updated: 2024/10/01 17:44:41 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	cd_in_env(t_var *data, char *pwd)
{
	char	*tmp;

	if (replace_var(data, "PWD", pwd) == false)
	{
		tmp = ft_strjoin("PWD=", pwd);
		if (!tmp)
			return (MALLOC_FAIL);
		if (add_var_to_env(data, tmp) == MALLOC_FAIL)
			return (free(tmp), MALLOC_FAIL);
		free(tmp);
	}
	if (replace_var(data, "OLDPWD", data->pwd) == false)
	{
		tmp = ft_strjoin("OLDPWD=", data->pwd);
		if (!tmp)
			return (MALLOC_FAIL);
		if (add_var_to_env(data, tmp) == MALLOC_FAIL)
			return (free(tmp), MALLOC_FAIL);
		free(tmp);
	}
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
