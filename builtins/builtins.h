/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:28:57 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 15:37:41 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	ft_cd(t_var *data, t_ast *token);
void	ft_echo(t_var *data, t_ast *token);
void	ft_env(t_var *data);
void	ft_exit(t_var *data);
void	ft_export(t_var *data, t_ast *token);
void	ft_pwd(t_var *data);
void	ft_unset(t_var *data, t_ast *token);

void	del_var(t_env **env, char *key);
t_env	*create_new_var(char *key, char *content);
void	add_var_to_back(t_env **env, t_env *new);

#endif