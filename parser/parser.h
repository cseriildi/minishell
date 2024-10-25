/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:22:01 by icseri            #+#    #+#             */
/*   Updated: 2024/10/25 12:32:47 by icseri           ###   ########.fr       */
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

//TABLE UTILS
t_table	*create_table(void);
t_table	*table_new(char **args);
void	table_add_back(t_table **lst, t_table *new);
void	table_clear(t_table **lst);
void	free_tab(char **tabb);
void	free_table(t_table **p_table);

//SHIFT REDUCE
int		shift(t_stack **stack, t_token **token_list, int next_state);
int		reduce(t_stack **stack, t_table *p_table, t_table *entry, t_stack **r);

//STACK OP
t_stack	*init_stack(void);
t_stack	*new_stack_node(t_token *token);
void	stack_add_front(t_stack **stack, t_stack *new);
t_stack	*pop_stack(t_stack **stack);
void	del_stack_node(t_stack **stack);
int		get_next_state(t_table *p_table, t_stack *stack);
void	free_stack(t_stack **stack);
void	stack_reverse(t_stack **stack);
t_stack	*copy_node(t_stack *node);
void	swap_stack(t_stack **a, t_stack **b);

//DEBUG UTILS
void	print_stack(t_stack *stack);
void	print_p_table(t_table *parsing_table);

//EXEC
t_exec	*create_exec(t_stack **res);
void	print_exec(t_exec *exec);
void	reverse_down_up(t_exec **exec);
void	reverse_front_back(t_exec **exec);
void	arrange_in_pip_seq(t_exec **exec);
void	sort_in_pip_seq_util(t_exec **prev_d, t_exec **curr_d);
void	sort_in_pip_seq(t_exec **exec);
void	reverse_exec(t_exec **exec);
int		exec_last_is_not_cmd(t_exec *exec);
void	exec_add_back_under(t_exec **where_a, t_exec *what);
void	exec_add_under(t_exec **where_a, t_exec *what);
void	exec_add_back(t_exec **where_a, t_exec *what);
t_exec	*exec_new(t_stack **res);
t_exec	*exec_return_others(t_stack **res, t_exec *new_node);
t_exec	*exec_return_pipe(t_exec *new_node, t_stack **res);
void	put_pipes_right_place(t_stack **result);
void	free_exec_all(t_exec **exec);

#endif