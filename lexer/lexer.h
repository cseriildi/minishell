/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:33:55 by icseri            #+#    #+#             */
/*   Updated: 2024/07/23 20:21:57 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

//list
t_token	*create_new_token(char *content, int type);
void	add_token_to_back(t_token **lst, t_token *new);

//utils
char	*get_word(char *text, char *separator);

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