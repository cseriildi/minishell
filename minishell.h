/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:28 by icseri            #+#    #+#             */
/*   Updated: 2024/07/20 15:21:10 by pvass            ###   ########.fr       */
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
	END = 0,
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

typedef struct s_var
{
	t_token	**tokens;
	char	*line;
	int		index;
	pid_t	pid;
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

/*TOKENS*/

typedef enum e_parser_action
{
	A_ACCEPT		= 0,
	A_SHIFT			,
	A_REDUCE		,
	A_GOTO			,
}	t_prs_act;

typedef enum e_parser_element
{
	P_END			= 0,
	P_NONE			= 1,
	P_WORD			= 2,
	P_ASSIGNMENT_WORD,
	P_RED_IN,
	P_RED_OUT,
	P_PIPE,
	P_HERE_DOC,
	P_APPEND,
	P_OR,
	P_AND,
	P_L_BRACKET,
	P_R_BRACKET,
	P_AND_OR		= 100,
	P_PIPE_SEQ,
	P_CMD,
	P_SUBSHELL,
	P_SIMPLE_CMD,
	P_CMD_NAME,
	P_CMD_WORD,
	P_CMD_PREFIX,
	P_CMD_SUFFIX,
	P_RED_LIST,
	P_IO_RED,
	P_IO_FILE,
	P_FILENAME,
	P_IO_HERE,
	P_HERE_END
}	t_prs_elem;

typedef struct s_table
{
	int				state;
	int				event;
	int				action;
	int				next_s;
	int				nb_reduce;
	struct s_table	*next;
}		t_table;

typedef struct s_stack
{
	int				type;
	char			*data;
	int				state;
	struct s_stack	*next;
}	t_stack;

typedef struct s_ast
{
	int				type;
	char			*data;
	struct s_ast	*child;
	struct s_ast	*next;
}	t_ast;

//list
t_token	*create_new_token(char *content, int type);
void	add_token_to_back(t_token **lst, t_token *new);
void	free_tokens(t_token **tokens);

//lexer
void	lexer(t_var *data);

//utils
char	*get_word(char *text, char *separator);
void	safe_exit(t_var *data, int exit_code);

//check line
void	check_brackets(t_var *data);

//word
void	single_quote(t_var *data);
char	*fix_content(char *content, t_var *data);
void	double_quote(t_var *data);
void	word(t_var *data);

//other types
void	redirection(t_var *data);
void	input(t_var *data);
void	pipes(t_var *data);
void	braces(t_var *data);
void	and_sign(t_var *data);

/* PARSE */

void	parse(t_table *p_table, t_token **token_list);

/* TABLE UTILS */

t_table *create_table();

t_table	*table_new(char **args);

void	table_add_back(t_table **lst, t_table *new);

void	table_clear(t_table **lst);

void	free_tab(char **tabb);

/*SHIFT REDUCE*/

int	shift(t_stack **stack, t_token **token_list, int next_state);

int reduce(t_stack **stack, t_table *p_table, t_table *entry);

/*STACK OP*/

t_stack *init_stack();

t_stack	*new_stack_node(t_token *token);

void	stack_add_front(t_stack **stack, t_stack *new);

t_stack *pop_stack(t_stack **stack);

void	del_stack_node(t_stack **stack);



int get_next_state(t_table *p_table, t_stack *stack);

/*DEBUG UTILS*/

void	print_stack(t_stack *stack);

void	print_p_table(t_table *parsing_table);

#endif
