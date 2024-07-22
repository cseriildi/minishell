/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 10:52:08 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 15:55:59 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	cd_in_env(t_env **env, char *pwd, char *oldpwd)
{
	t_env	*head;
	t_env	*curr;

	head = *env;
	curr = head;
	while (curr != NULL)
	{
		if (ft_strncmp(curr->key, "PWD", 4) == 0)
		{
			ft_free(&curr->content);
			curr->content = pwd;
		}
		if (ft_strncmp(curr->key, "OLDPWD", 4) == 0)
		{
			ft_free(&curr->content);
			curr->content = oldpwd;
		}
		curr = curr->next;
	}
}

void	ft_cd(t_var *data, t_ast *token)
{
	char	*pwd;
	char	*oldpwd;

	oldpwd = data->pwd;
/* 	if (access(token->data, F_OK) == -1)
		exit(CANNOT_OPEN_FILE); */
	chdir(token->data);
	ft_free(&data->pwd);
	pwd = getcwd(NULL, 0);
	cd_in_env(data->env, pwd, oldpwd);
	ft_free(&data->pwd);
	data->pwd = pwd;
	exit(EXIT_SUCCESS);
}
