/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:54:43 by icseri            #+#    #+#             */
/*   Updated: 2024/10/01 15:46:23 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	word(t_var *data)
{
	t_token	*new_token;
	char	*content;
	int		end_index;

	end_index = find_end_of_word(data, data->line + data->index);
	if (data->missing_quote == true)
		return ;
	content = ft_substr(data->line, data->index, end_index);
	if (!content)
		safe_exit(data, MALLOC_FAIL);
	data->index += ft_strlen(content);
	new_token = create_new_token(content, WORD);
	free(content);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(&data->tokens, new_token);
}

int	find_end_of_quote(t_var *data, char *line)
{
	int		index;
	char	quote;

	quote = *line;
	index = 1;
	while (line[index] && line[index] != quote)
		index++;
	if (line[index] == '\0')
	{
		if (quote == '\'')
			print_error(1, "minishell: syntax error: missing \'\'\'");
		else
			print_error(1, "minishell: syntax error: missing \'\"\'");
		data->missing_quote = true;
		data->exit_code = ERROR_MISUSE;
	}
	return (index);
}

int	find_end_of_word(t_var *data, char *line)
{
	char	*letter;
	int		index;
	
	index = 0;
	while (line[index])
	{
		if (line[index] ==  '\'' || line[index] == '\"')
			index += find_end_of_quote(data, line + index);
		else
		{
			letter = ft_strchr(" \t\n\v\f\r|><", line[index]);
			if (letter && *letter)
				break ;
		}
		if (data->missing_quote == true)
			break ;
		index++;
	}
	return (index);
}
