/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:46 by icseri            #+#    #+#             */
/*   Updated: 2024/10/22 08:34:56 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../minishell.h"

typedef struct s_stack
{
	int				type;
	char			*data;
	int				state;
	struct s_stack	*next;
}	t_stack;
t_exec	*exec_new(t_stack **res);

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
bool	redirect_in(t_var *data, t_exec *tree, int read_fd);
bool	redirect_out(t_var *data, t_exec *tree, int write_fd);
int		is_directory(const char *path);

void	do_heredoc(t_var *data, char *limiter, bool expanding);
void	heredoc(t_var *data, t_exec *exec);

int		get_chunk_size(char *str);

t_token	*create_new_token(char *content, int type);
void	add_token_to_back(t_token **lst, t_token *new);
void	fix_exec(t_var *data, t_exec *exec);
int add_chunk(t_var *data, char *str, bool to_join);
t_exec *create_exec_node(char *content);

#endif
