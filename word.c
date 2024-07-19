/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:54:43 by icseri            #+#    #+#             */
/*   Updated: 2024/07/19 17:44:03 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free(content);
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

	first = get_word(content, "$");
	if (!first)
	{
		free(content);
		safe_exit(data, MALLOC_FAIL);
	}
	var_name = get_word(ft_strchr(content, '$') + 1, " ()$|><\'\"&");
	if (!var_name)
	{
		free(content);
		free(first);
		safe_exit(data, MALLOC_FAIL);
	}
	expanded_var = getenv(var_name);
	if (!expanded_var)
	{
		free(content);
		free(first);
		free(var_name);
		safe_exit(data, VARIABLE_NOT_FOUND);
	}
	tmp = ft_strjoin(first, expanded_var);
	if (!tmp)
	{
		free(content);
		free(first);
		free(var_name);
		safe_exit(data, MALLOC_FAIL);
	}
	last = ft_strdup(content + ft_strlen(first) + ft_strlen(var_name) + 1);
	free(content);
	free(first);
	free(var_name);
	if (!last)
	{
		free(tmp);
		safe_exit(data, MALLOC_FAIL);
	}
	content = ft_strjoin(tmp, last);
	free(tmp);
	free(last);
	if (!content)
		safe_exit(data, MALLOC_FAIL);
	return (content);
}

void	double_quote(t_var *data)
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
	while (ft_strchr(content, '$') != NULL)
		content = fix_content(content, data);
	new_token = create_new_token(content, WORD);
	free(content);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
}

void	word(t_var *data)
{
	t_token	*new_token;
	char	*content;
	char	*expanded_var;

	if (data->line[data->index] == '$')
	{
		data->index++;
		content = get_word(data->line + data->index, " ()$|><\'\"&");
		if (!content)
			safe_exit(data, MALLOC_FAIL);
		expanded_var = getenv(content);
		if (!expanded_var)
		{
			free(content);
			safe_exit(data, VARIABLE_NOT_FOUND);
		}
		new_token = create_new_token(expanded_var, WORD);
	}
	else
	{
		content = get_word(data->line + data->index, " ()$|><\'\"&");
		if (!content)
			safe_exit(data, MALLOC_FAIL);
		new_token = create_new_token(content, WORD);
	}
	data->index += ft_strlen(content);
	free(content);
	if (!new_token)
		safe_exit(data, MALLOC_FAIL);
	add_token_to_back(data->tokens, new_token);
}
