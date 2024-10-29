/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:46 by icseri            #+#    #+#             */
/*   Updated: 2024/10/29 21:06:30 by pvass            ###   ########.fr       */
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
void	fix_content(t_var *data, t_exec *seq, bool expandable);
int		expand(char *content, t_var *data, bool starts_with_dollar);
bool	redirect_in(t_var *data, t_exec *tree, int *read_fd);
bool	redirect_out(t_var *data, t_exec *tree, int *write_fd);
bool	is_directory(const char *path);

bool	do_heredoc(t_var *data, char *limiter, bool expanding);
bool	heredoc(t_var *data, t_exec *exec);

int		get_chunk_size(char *str);

t_token	*create_new_token(char *content, int type);
void	add_token_to_back(t_token **lst, t_token *new);
void	fix_exec(t_var *data, t_exec *exec);
int		add_chunk(t_var *data, char *str, bool to_join);
t_exec	*create_exec_node(char *content);
char	*get_var_name(char *str);

//UTILS
int		is_in_out_app(t_exec *exec);
int		count_exec(t_exec *exec);
bool	good_redir_path(char *redir, t_var *data);
bool	is_directory(const char *path);
char	**get_paths(t_var *data);
bool	redirs_exist(t_var *data, t_exec *exec);

char	*get_chunk(t_var *data, char *str, int index, int len);
int		join_to_last(t_token *curr, char *str);

void	last_sequence_child(t_var *data, t_exec *exec);
void	only_one_seq_child(t_var *data, t_exec *exec);

#endif
