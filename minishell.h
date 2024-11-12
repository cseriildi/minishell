/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:28 by icseri            #+#    #+#             */
/*   Updated: 2024/11/12 20:22:44 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200112L
# define _GNU_SOURCE

# include "libft/libft.h"
# include <stdio.h> 
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
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
# include <signal.h>
# include <sys/types.h>

# define READ 0
# define CREATE 1
# define CREATE_OR_APPEND 2
# define WHITE_SPACES " \t\n\v\f\r"
# define SEPARATORS " \t\n\v\f\r|><"

extern int	g_sig_num;

typedef enum s_sigstatus
{
	MAIN,
	HEREDOC,
	IN_COMMAND,
}	t_sigstatus;

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
	t_token	*command_list;
	t_token	*heredoc_input;
	bool	is_heredoc;
	char	*limiter;
	char	**cmd_list;
	char	*line;
	int		index;
	pid_t	pid;
	int		pipe1_fd[2];
	int		pipe2_fd[2];
	int		exit_status;
	int		exit_code;
	char	*pwd;
	char	*prompt;
	char	**env;
	int		proc_count;
	bool	missing_quote;
	int		fd_to_write;
	int		to_join;
	int		has_child;
}	t_var;

typedef struct s_signals
{
	t_var	*data;
	pid_t	child_pid;
	int		interactive;
}	t_signals;

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
	ERRORS_AFTER_EXECVE = 126,
	COMMAND_NOT_FOUND = 127,
}	t_err;

typedef struct s_stack
{
	int				type;
	char			*data;
	int				state;
	struct s_stack	*next;
}	t_stack;

typedef enum e_signal_state
{
	SIG_DEFAULT		= 0,
	SIG_IGNORE,
	SIG_STANDARD,
	SIG_RECORD
}	t_sig;

//init
void		init(t_var *data);
void		get_prompt(t_var *data);
char		*safe_getcwd(t_var *data);

//utils2
void		read_input(t_var *data);
void		malloc_failed(t_var *data);
char		*ft_strjoin2(char *str1, char *str2, char *delimiter);
int			ft_abs(char c);
char		*read_heredoc(char *limiter, int len);

//lexer
void		lexer(t_var *data);

//parser
void		parse(t_var *data);
t_table		*create_table(void);
t_exec		*exec_new(t_stack **res);
t_exec		*create_exec_node(char *content);

//utils
void		free_all(t_var *data);
void		safe_exit(t_var *data, int exit_code);
void		free_tokens(t_token **token);
void		free_array(char ***arr);
void		print_error(int count, ...);

//expand utils
char		**easy_split(char *str, char *delim);
char		*get_var_name(char *str);

//builtins
bool		exec_builtin(t_var *data);
bool		is_builtin(char *cmd);

//executor
void		execute(t_var *data);
void		init_env(t_var *data);
char		*ft_getenv(t_var *data, char *var_name, bool is_safe);

//exec_utils
void		free_table(t_table **p_table);
void		free_exec_all(t_exec **exec);
void		print_exec(t_exec *exec);

//fd_handling
int			safe_open(char *filename, int mode, t_var *data);
void		safe_close(int *fd);
void		delete_file(t_var *data);
void		safe_dup2(int *old_fd, int new_fd, t_var *data);

//atol itol
long long	ft_atoll(const char *str);
char		*ft_lltoa(long long nb);
bool		is_ll_overflow(t_var *data, char *str);

//signal
void		sig_setup(int signo, t_sig state);
void		handle_signal_std(int signo, siginfo_t *info, void *context);

char		**path_split(char *str, char c);

void		sig_hand(int type);

#endif
