/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:28:57 by icseri            #+#    #+#             */
/*   Updated: 2024/09/17 16:06:04 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	ft_cd(t_var *data, char **cmd_list);
void	ft_echo(t_var *data, char **cmd_list);
void	ft_env(t_var *data);
void	ft_exit(t_var *data, char **cmd_list);
void	ft_export(t_var *data, char **cmd_list);
void	ft_pwd(t_var *data);
void	ft_unset(t_var *data, char **cmd_list);

int		add_var_to_env(t_var *data, char *line);
bool	replace_var(t_var *data, char *key, char *content);

char	*get_word(char *text, char *separator);

#endif