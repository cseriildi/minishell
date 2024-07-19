/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:28 by icseri            #+#    #+#             */
/*   Updated: 2024/07/19 17:41:07 by icseri           ###   ########.fr       */
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

typedef enum s_token_type
{
	END,
	WORD,
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

#endif