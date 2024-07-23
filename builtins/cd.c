/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:08 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 19:35:50 by cseriildii       ###   ########.fr       */
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

void	ft_cd(t_var *data, t_ast *token)
{
	char	*pwd;

	if (chdir(token->data) == -1)
	{
		print_error(4, "minishell: cd: ", token->data, ": ", strerror(errno));
		data->exit_code = ERROR_MISUSE;
	}
	ft_free(&data->pwd);
	pwd = getcwd(NULL, 0);
	if (cd_in_env(data, pwd) == MALLOC_FAIL)
	{
		data->exit_code = MALLOC_FAIL;
		ft_free(&pwd);
		print_error(1, strerror(errno));
	}
	ft_free(&data->pwd);
	data->pwd = pwd;
	data->exit_code = EXIT_SUCCESS;
}
