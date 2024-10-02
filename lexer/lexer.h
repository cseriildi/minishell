/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:33:55 by icseri            #+#    #+#             */
/*   Updated: 2024/10/02 11:21:06 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

//list
t_token	*create_new_token(char *content, int type);
void	add_token_to_back(t_token **lst, t_token *new);

//word
void	word(t_var *data);
int		find_end_of_quote(t_var *data, char *line);
int		find_end_of_word(t_var *data, char *line);

//other types
void	redirection(t_var *data);
void	input(t_var *data);
void	pipes(t_var *data);

#endif