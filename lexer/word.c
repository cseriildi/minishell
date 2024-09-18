/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:54:43 by icseri            #+#    #+#             */
/*   Updated: 2024/09/18 14:00:31 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
/* 
void	single_quote(t_var *data)
{
	char	*content;
	t_token	*new_token;

	data->index++;
	if (ft_strchr(data->line + data->index, '\'') == NULL)
		safe_exit(data, MALLOC_FAIL);
	content = get_word(data->line + data->index, "\'");
	if (!content)
		safe_exit(data, MALLOC_FAIL);
	data->index += ft_strlen(content) + 1;
	new_token = create_new_token(content, WORD);
	ft_free(&content);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
}

char	*fix_content(char *content, t_var *data)
{
	char	*first;
	char	*var_name;
	char	*last;
	char	*tmp;
	char	*expanded_var;

	while (ft_strchr(content, '$') != NULL)
	{
		first = get_word(content, "$");
		if (!first)
		{
			ft_free(&content);
			safe_exit(data, MALLOC_FAIL);
		}
		var_name = get_word(ft_strchr(content, '$') + 1, " ()$|><\'\"&");
		if (!var_name)
		{
			ft_free(&content);
			ft_free(&first);
			safe_exit(data, MALLOC_FAIL);
		}
		if (*var_name == '?')
			expanded_var = ft_itoa(data->exit_code);
		else
			expanded_var = ft_strdup(ft_getenv(data, var_name));
		if (!expanded_var)
		{
			ft_free(&content);
			ft_free(&first);
			ft_free(&var_name);
			safe_exit(data, MALLOC_FAIL);
		}
		tmp = ft_strjoin(first, expanded_var);
		ft_free(&expanded_var);
		if (!tmp)
		{
			ft_free(&content);
			ft_free(&first);
			ft_free(&var_name);
			safe_exit(data, MALLOC_FAIL);
		}
		if (*var_name == '?')
			last = ft_strdup(content + ft_strlen(first) + 2);
		else
			last = ft_strdup(content 
				+ ft_strlen(first) + ft_strlen(var_name) + 1);
		ft_free(&content);
		ft_free(&first);
		ft_free(&var_name);
		if (!last)
		{
			ft_free(&tmp);
			safe_exit(data, MALLOC_FAIL);
		}
		content = ft_strjoin(tmp, last);
		ft_free(&tmp);
		ft_free(&last);
		if (!content)
			safe_exit(data, MALLOC_FAIL);
	}
	return (content);
}
 */
/* void	double_quote(t_var *data)
{
	char	*content;
	t_token	*new_token;

	data->index++;
	if (ft_strchr(data->line + data->index, '\"') == NULL)
		safe_exit(data, ERROR_MISUSE);
	content = get_word(data->line + data->index, "\"");
	if (!content)
		safe_exit(data, MALLOC_FAIL);
	data->index += ft_strlen(content) + 1;
	content = fix_content(content, data);
	new_token = create_new_token(content, WORD);
	ft_free(&content);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
} */

void	word(t_var *data)
{
	t_token	*new_token;
	char	*content;

	content = get_word(data->line + data->index, " |><");
	if (!content)
		safe_exit(data, MALLOC_FAIL);
	data->index += ft_strlen(content);
	new_token = create_new_token(content, WORD);
	ft_free(&content);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
}
