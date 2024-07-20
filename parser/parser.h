/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:22:01 by icseri            #+#    #+#             */
/*   Updated: 2024/07/20 16:08:37 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../minishell.h"

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

//PARSE
void	parse(t_table *p_table, t_token **token_list);

//TABLE UTILS
t_table	*create_table(void);
t_table	*table_new(char **args);
void	table_add_back(t_table **lst, t_table *new);
void	table_clear(t_table **lst);
void	free_tab(char **tabb);

//SHIFT REDUCE
int		shift(t_stack **stack, t_token **token_list, int next_state);
int		reduce(t_stack **stack, t_table *p_table, t_table *entry);

//STACK OP
t_stack	*init_stack(void);
t_stack	*new_stack_node(t_token *token);
void	stack_add_front(t_stack **stack, t_stack *new);
t_stack	*pop_stack(t_stack **stack);
void	del_stack_node(t_stack **stack);
int		get_next_state(t_table *p_table, t_stack *stack);

//DEBUG UTILS
void	print_stack(t_stack *stack);
void	print_p_table(t_table *parsing_table);

#endif