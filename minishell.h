/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:28 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 15:32:13 by icseri           ###   ########.fr       */
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
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <fcntl.h>

# define PARSING_TABLE "./parser/parse_table_numbs"

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

typedef struct s_ast
{
	int				type;
	char			*data;
	struct s_ast	*child;
	struct s_ast	*next;
}	t_ast;

typedef struct s_env
{
	char			*key;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_var
{
	t_token	**tokens;
	t_table	*p_table;
	t_ast	**tree;
	char	*line;
	int		index;
	pid_t	pid;
	int		exit_status;
	int		exit_code;
	char	*pwd;
	char	*promt;
	t_env	**env;
	int		subshell_level;
}	t_var;

typedef enum s_err
{
	ERROR_MISUSE = 2,
	READLINE_FAIL,
	CANNOT_OPEN_FILE,
	MALLOC_FAIL,
	VARIABLE_NOT_FOUND,
	PIPE_FAIL,
	DUP2_FAIL,
	FORK_FAIL,
	UNLINK_FAIL,
	COMMAND_NOT_FOUND = 127
}	t_err;

//lexer
void	lexer(t_var *data);

//parser
void	parse(t_table *p_table, t_token **tokens);
t_table	*create_table(void);

//utils
void	safe_exit(t_var *data, int exit_code);
void	check_brackets(t_var *data);
void	get_promt(t_var *data);
void	free_tokens(t_var *data);
void	free_array(char **arr);

//builtins
void	builtin(t_var *data, t_ast *token);

//executor
void	execute(t_var *data);

//env
void	init_env(t_var *data);
char	*ft_getenv(t_var *data, char *var_name);
void	free_env(t_env **env);

#endif
