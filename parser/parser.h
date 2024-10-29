/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:22:01 by icseri            #+#    #+#             */
/*   Updated: 2024/10/29 19:31:55 by icseri           ###   ########.fr       */
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

# define PARSE_LINE "0	0	1	24	-1\n\
0	2	1	1	-1\n\
0	4	1	2	-1\n\
0	5	1	3	-1\n\
0	7	1	4	-1\n\
0	8	1	5	-1\n\
0	11	1	6	-1\n\
0	100	3	7	-1\n\
0	101	3	8	-1\n\
0	102	3	9	-1\n\
0	103	3	10	-1\n\
0	104	3	11	-1\n\
0	105	3	12	-1\n\
0	107	3	13	-1\n\
0	110	3	14	-1\n\
0	111	3	15	-1\n\
0	113	3	16	-1\n\
1	1	2	105	1\n\
2	2	1	17	-1\n\
2	112	3	18	-1\n\
3	2	1	17	-1\n\
3	112	3	19	-1\n\
4	2	1	20	-1\n\
4	114	3	21	-1\n\
5	2	1	17	-1\n\
5	112	3	22	-1\n\
6	2	1	1	-1\n\
6	4	1	2	-1\n\
6	5	1	3	-1\n\
6	7	1	4	-1\n\
6	8	1	5	-1\n\
6	11	1	6	-1\n\
6	100	3	23	-1\n\
6	101	3	8	-1\n\
6	102	3	9	-1\n\
6	103	3	10	-1\n\
6	104	3	11	-1\n\
6	105	3	12	-1\n\
6	107	3	13	-1\n\
6	110	3	14	-1\n\
6	111	3	15	-1\n\
6	113	3	16	-1\n\
7	0	1	24	-1\n\
7	10	1	25	-1\n\
7	9	1	26	-1\n\
8	6	1	27	-1\n\
8	1	2	100	1\n\
9	1	2	101	1\n\
10	4	1	2	-1\n\
10	5	1	3	-1\n\
10	7	1	4	-1\n\
10	8	1	5	-1\n\
10	1	2	102	1\n\
10	109	3	28	-1\n\
10	110	3	29	-1\n\
10	111	3	15	-1\n\
10	113	3	16	-1\n\
11	1	2	102	1\n\
12	2	1	30	-1\n\
12	4	1	2	-1\n\
12	5	1	3	-1\n\
12	7	1	4	-1\n\
12	8	1	5	-1\n\
12	1	2	104	1\n\
12	108	3	31	-1\n\
12	110	3	32	-1\n\
12	111	3	15	-1\n\
12	113	3	16	-1\n\
13	2	1	33	-1\n\
13	4	1	2	-1\n\
13	5	1	3	-1\n\
13	7	1	4	-1\n\
13	8	1	5	-1\n\
13	1	2	104	1\n\
13	106	3	34	-1\n\
13	110	3	35	-1\n\
13	111	3	15	-1\n\
13	113	3	16	-1\n\
14 	1	2	107	1\n\
15	1	2	110	1\n\
16	1	2	110	1\n\
17	1	2	112	1\n\
18	1	2	111	2\n\
19	1	2	111	2\n\
20	1	2	114	1\n\
21	1	2	113	2\n\
22	1	2	111	2\n\
23	10	1	25	-1\n\
23	9	1	26	-1\n\
23	12	1	36	-1\n\
24	1	0	-1	-1\n\
25	2	1	1	-1\n\
25	4	1	2	-1\n\
25	5	1	3	-1\n\
25	7	1	4	-1\n\
25	8	1	5	-1\n\
25	11	1	6	-1\n\
25	101	3	37	-1\n\
25	102	3	9	-1\n\
25	103	3	10	-1\n\
25	104	3	11	-1\n\
25	105	3	12	-1\n\
25	107	3	13	-1\n\
25	110	3	14	-1\n\
25	111	3	15	-1\n\
25	113	3	16	-1\n\
26	2	1	1	-1\n\
26	4	1	2	-1\n\
26	5	1	3	-1\n\
26	7	1	4	-1\n\
26	8	1	5	-1\n\
26	11	1	6	-1\n\
26	101	3	38	-1\n\
26	102	3	9	-1\n\
26	103	3	10	-1\n\
26	104	3	11	-1\n\
26	105	3	12	-1\n\
26	107	3	13	-1\n\
26	110	3	14	-1\n\
26	111	3	15	-1\n\
26	113	3	16	-1\n\
27	2	1	1	-1\n\
27	4	1	2	-1\n\
27	5	1	3	-1\n\
27	7	1	4	-1\n\
27	8	1	5	-1\n\
27	11	1	6	-1\n\
27	102	3	39	-1\n\
27	103	3	10	-1\n\
27	104	3	11	-1\n\
27	105	3	12	-1\n\
27	107	3	13	-1\n\
27	110	3	14	-1\n\
27	111	3	15	-1\n\
27	113	3	16	-1\n\
28	4	1	2	-1\n\
28	5	1	3	-1\n\
28	7	1	4	-1\n\
28	8	1	5	-1\n\
28	1	2	102	2\n\
28	110	3	40	-1\n\
28	111	3	15	-1\n\
28	113	3	16	-1\n\
29	1	2	109	1\n\
30	1	2	108	1\n\
31	2	1	41	-1\n\
31	4	1	2	-1\n\
31	5	1	3	-1\n\
31	7	1	4	-1\n\
31	8	1	5	-1\n\
31	1	2	104	2\n\
31	110	3	42	-1\n\
31	111	3	15	-1\n\
31	113	3	16	-1\n\
32	1	2	108	1\n\
33	1	2	106	1\n\
34	2	1	30	-1\n\
34	4	1	2	-1\n\
34	5	1	3	-1\n\
34	7	1	4	-1\n\
34	8	1	5	-1\n\
34	1	2	104	2\n\
34	108	3	43	-1\n\
34	110	3	32	-1\n\
34	111	3	15	-1\n\
34	113	3	16	-1\n\
35	1	2	107	2\n\
36	1	2	103	3\n\
37	6	1	27	-1\n\
37	1	2	100	3\n\
38	6	1	27	-1\n\
38	1	2	100	3\n\
39	1	2	101	3\n\
40	1	2	109	2\n\
41	1	2	108	2\n\
42	1	2	108	2\n\
43	2	1	41	-1\n\
43	4	1	2	-1\n\
43	5	1	3	-1\n\
43	7	1	4	-1\n\
43	8	1	5	-1\n\
43	1	2	104	3\n\
43	110	3	42	-1\n\
43	111	3	15	-1\n\
43	113	3	16	-1"

#endif