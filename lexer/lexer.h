/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:33:55 by icseri            #+#    #+#             */
/*   Updated: 2024/09/24 15:55:02 by icseri           ###   ########.fr       */
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
char	*fix_content(char *content, t_var *data);
void	word(t_var *data);

//other types
void	redirection(t_var *data);
void	input(t_var *data);
void	pipes(t_var *data);

#endif