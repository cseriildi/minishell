# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/16 14:33:31 by icseri            #+#    #+#              #
#    Updated: 2024/09/17 12:44:38 by cseriildii       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = main.c \
		utils.c \
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
		executor/execute.c \
		builtins/env_utils.c \
		builtins/builtins.c \
		builtins/cd.c \
		builtins/echo.c \
		builtins/env.c \
		builtins/exit.c \
		builtins/export.c \
		builtins/pwd.c \
		builtins/unset.c

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

built: all clean

.PHONY: all clean fclean re
