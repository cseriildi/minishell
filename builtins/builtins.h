/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:28:57 by icseri            #+#    #+#             */
/*   Updated: 2024/09/30 15:49:56 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	ft_cd(t_var *data);
void	ft_echo(t_var *data);
void	ft_env(t_var *data);
void	ft_exit(t_var *data);
void	ft_export(t_var *data);
void	ft_pwd(t_var *data);
void	ft_unset(t_var *data);

int		add_var_to_env(t_var *data, char *line);
bool	replace_var(t_var *data, char *key, char *content);

bool	is_valid_var(t_var *data, char *line);
void	no_arg_export(t_var *data);
void	ex_util2(t_var *data);

char	*get_word(char *text, char *separator);

#endif