/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:46 by icseri            #+#    #+#             */
/*   Updated: 2024/10/02 14:10:25 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../minishell.h"

void	only_one_sequence(t_var *data, t_exec *tree);
void	first_sequence(t_var *data, t_exec *tree);
void	middle_sequence(t_var *data, t_exec *tree);
void	last_sequence(t_var *data, t_exec *tree);
void	exec_sequence(t_var *data, t_exec *tree, int read, int write);
void	create_cmd_list(t_var *data, t_exec *tree);
void	exec_command(t_var *data);
bool	is_builtin(char *cmd);
char	**get_paths(t_var *data);
char	*get_abs_cmd(t_var *data, char *cmd);
char	*expander(t_var *data, char *str);
bool	redirect_in(t_var *data, t_exec *tree);
bool	redirect_out(t_var *data, t_exec *tree);
int is_directory(const char *path);

bool	here_doc(t_var *data, char *limiter, bool expanding);

int		get_chunk_size(char *str);
#endif
