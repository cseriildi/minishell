/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:28 by icseri            #+#    #+#             */
/*   Updated: 2024/09/30 15:01:38 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h> 
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <wait.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <fcntl.h>
# include <termios.h>

# define PARSING_TABLE "./parser/parse_table_numbs"

# define READ 0
# define CREATE 1
# define CREATE_OR_APPEND 2

typedef enum s_token_type
{
	END,
	NONE,
	WORD,
	ASSIGNMENT_WORD,
	RED_IN,
	RED_OUT,
	PIPE,
	HERE_DOC,
	APPEND,
	OR,
	AND,
	L_BRACKET,
	R_BRACKET,
}	t_token_type;

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_table
{
	int				state;
	int				event;
	int				action;
	int				next_s;
	int				nb_reduce;
	struct s_table	*next;
}	t_table;

typedef struct s_exec
{
	int				type;
	char			*data;
	struct s_exec	*down;
	struct s_exec	*next;
}	t_exec;

typedef struct s_env
{
	char			*line;
	char			*key;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_var
{
	t_token	*tokens;
	t_exec	*exec;
	t_table	*p_table;
	char	**cmd_list;
	char	*line;
	int		index;
	pid_t	pid;
	int		pipe1_fd[2];
	int		pipe2_fd[2];
	int		exit_status;
	int		exit_code;
	char	*pwd;
	char	*promt;
	char	**env;
	int		subshell_level;
	int		stdout_copy;
	char	*here_doc_filename;
	bool	missing_quote;
}	t_var;

typedef struct s_signals
{
	pid_t	child_pid;
	int		interactive;
	int		exit_c;
}	t_signals;

extern t_signals	signals;

typedef enum s_err
{
	ERROR_MISUSE = 2,
	READLINE_FAIL,
	CANNOT_OPEN_FILE,
	MALLOC_FAIL,
	PIPE_FAIL,
	DUP2_FAIL,
	FORK_FAIL,
	UNLINK_FAIL,
	NON_NUMERIC_EXIT,
	COMMAND_NOT_FOUND = 127,
}	t_err;

//lexer
void	lexer(t_var *data);

//parser
void	parse(t_var *data);
t_table	*create_table(void);

//utils
void	free_all(t_var *data);
void	safe_exit(t_var *data, int exit_code);
void	free_tokens(t_var *data);
void	free_array(char ***arr);
void	print_error(int count, ...);

//builtins
bool	exec_builtin(t_var *data);
bool	is_builtin(char *cmd);

//executor
void	execute(t_var *data);

//env
void	init_env(t_var *data);
char	*ft_getenv(t_var *data, char *var_name);
char	*ft_strjoin2(char *str1, char *str2, char *delimiter);

//exec_utils
void	free_table(t_table **p_table);
void	free_exec_all(t_exec **exec);
void	print_exec(t_exec *exec);

void	init_signals(t_var *data);

//fd_handling

int		safe_open(char *filename, int mode, t_var *data);
void	safe_close(int *fd);
void	delete_file(t_var *data, char *filename);
void	safe_dup2(int *old_fd, int new_fd, t_var *data);

//init
void	init(t_var *data);
void	get_promt(t_var *data);

#endif
