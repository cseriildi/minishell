/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:28:57 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 20:22:30 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	ft_cd(t_var *data, t_ast *token);
void	ft_echo(t_var *data, t_ast *token);
void	ft_env(t_var *data);
void	ft_exit(t_var *data, t_ast *token);
void	ft_export(t_var *data, t_ast *token);
void	ft_pwd(t_var *data);
void	ft_unset(t_var *data, t_ast *token);

int		add_var_to_env(t_var *data, char *line);
bool	replace_var(t_var *data, char *key, char *content);

char	*get_word(char *text, char *separator);

#endif