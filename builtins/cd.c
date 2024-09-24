/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:08 by icseri            #+#    #+#             */
/*   Updated: 2024/09/24 15:03:42 by icseri           ###   ########.fr       */
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
	//handle when text is "" or " " or even "     "
	if (dest == NULL)
	{
		dest = ft_getenv(data, "HOME");
		if (ft_strncmp(dest, "", 2) == 0)
		{
			print_error(2, "minishell: cd: HOME not set");
			data->exit_code = EXIT_FAILURE;
			return ;
		}
	}
	else if (data->cmd_list[2] != NULL)
	{
		print_error(2, "minishell: cd: too many arguments");
		data->exit_code = EXIT_FAILURE;
		return ;
	}
	if (chdir(dest) == -1)
	{
		print_error(4, "minishell: cd: ", dest, ": ", strerror(errno));
		data->exit_code = EXIT_FAILURE;
		return ;
	}
	ft_free(&data->pwd);
	pwd = getcwd(NULL, 0);
	if (cd_in_env(data, pwd) == MALLOC_FAIL)
	{
		print_error(2, "minishell: cd: ", strerror(errno));
		data->exit_code = MALLOC_FAIL;
	}
	ft_free(&data->pwd);
	data->pwd = pwd;
}
