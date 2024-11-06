# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pvass <pvass@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/16 14:33:31 by icseri            #+#    #+#              #
#    Updated: 2024/11/06 11:38:56 by pvass            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = main.c \
		utils.c \
		utils2.c \
		init.c \
		path_split.c \
		exec_utils.c \
		p_table_utils.c \
		fd_handling.c \
		atol_ltoa.c \
		lexer/lexer.c \
		lexer/list.c \
		lexer/other_types.c \
		lexer/utils.c \
		lexer/word.c \
		parser/debug_utils.c \
		parser/init_parsing_table.c \
		parser/parse.c \
		parser/parsing_table_utils.c \
		parser/shift_reduce.c \
		parser/stack_utils.c \
		parser/stack_free.c \
		parser/basic_functions.c \
		parser/create_exec_utils.c \
		parser/create_exec_utils_2.c \
		parser/arrange_exec.c \
		parser/arrange_exec_2.c \
		parser/put_pipes.c \
		executor/execute.c \
		executor/expander.c \
		executor/heredoc.c \
		executor/redirection.c \
		executor/expand_utils.c \
		executor/sequence_handling.c \
		executor/sequence_children.c \
		executor/execute_utils.c \
		executor/get_absolute_command.c \
		executor/create_command_list.c \
		executor/redirs_exist.c \
		executor/expand_utils2.c \
		builtins/env_utils.c \
		builtins/builtins.c \
		builtins/cd.c \
		builtins/echo.c \
		builtins/env.c \
		builtins/exit.c \
		builtins/export.c \
		builtins/export_util.c \
		builtins/pwd.c \
		builtins/unset.c \
		signal/sig.c

OBJS = ${SRCS:.c=.o}

COMP = cc

CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

all: ${NAME}

.c.o: 
	${COMP} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	@${MAKE} -C ./libft --no-print-directory
	@${COMP} ${CFLAGS} ${OBJS} ./libft/libft.a -o ${NAME} $(LDFLAGS)

clean: 
	@${MAKE} -C ./libft --no-print-directory fclean
	@rm -rf ${OBJS} ${BONUS_OBJS}

fclean: clean
	@rm -rf ${NAME} ${BONUS_NAME}
	
re: fclean all

build: all clean

.PHONY: all clean fclean re build
