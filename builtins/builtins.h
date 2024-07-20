/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:28:57 by icseri            #+#    #+#             */
/*   Updated: 2024/07/20 17:20:20 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	cd(t_var *data, t_ast *token);
void	echo(t_var *data, t_ast *token);
void	env(t_var *data);
void	exit(t_var *data);
void	export(t_var *data, t_ast *token);
void	pwd(t_var *data);
void	unset(t_var *data, t_ast *token);

#endif