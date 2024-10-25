/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:24:05 by icseri            #+#    #+#             */
/*   Updated: 2024/10/25 13:00:38 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	fix_content(t_var *data, t_exec *seq, bool expandable)
{
	int		index;
	int		len;
	char	*chunk;

	index = 0;
	len = 0;
	while (seq->data[index])
	{
		len = get_chunk_size(seq->data + index);
		chunk = get_chunk(data, seq->data, index, len);
		if (expandable && seq->data[index] != '\''
			&& !(len == 2 && seq->data[index] == '\"'))
		{
			if (expand(chunk, data, seq->data[index] == '\"') == MALLOC_FAIL)
				malloc_failed(data);
		}
		else
		{
			if (add_chunk(data, chunk, data->to_join++) == MALLOC_FAIL)
				return (ft_free(&chunk), malloc_failed(data));
			ft_free(&chunk);
		}
		index += len;
	}
	fix_exec(data, seq);
}

int	real_expand(t_var *data, bool is_quoted, char *var)
{
	char	**expanded_var;
	int		i;

	if (!*var || is_quoted || is_directory(var) == 1)
	{
		if ((is_quoted || is_directory(var) == 1)
			&& add_chunk(data, var, data->to_join++) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		return (EXIT_SUCCESS);
	}
	expanded_var = easy_split(var, WHITE_SPACES);
	if (!expanded_var)
		return (MALLOC_FAIL);
	data->to_join = ft_strchr(WHITE_SPACES, *var) == NULL;
	i = -1;
	while (expanded_var[++i])
	{
		if (add_chunk(data, expanded_var[i], data->to_join) == MALLOC_FAIL)
			return (free_array(&expanded_var), MALLOC_FAIL);
		data->to_join = 0;
	}
	data->to_join = ft_strchr(WHITE_SPACES, var[ft_strlen(var) - 1]) == NULL;
	free_array(&expanded_var);
	return (EXIT_SUCCESS);
}

int	expand_var(t_var *data, char *var, bool is_quoted)
{
	char	*number;

	if (!*var)
	{
		if (add_chunk(data, "$", data->to_join++) == MALLOC_FAIL)
			return (MALLOC_FAIL);
	}
	else if (*var == '?')
	{
		number = ft_itoa(data->exit_code);
		if (!number)
			return (MALLOC_FAIL);
		if (add_chunk(data, number, data->to_join++) == MALLOC_FAIL)
			return (ft_free(&number), MALLOC_FAIL);
		ft_free(&number);
	}
	else if (*var == ' ')
	{
		if (add_chunk(data, "$ ", data->to_join++) == MALLOC_FAIL)
			return (MALLOC_FAIL);
	}
	else if (real_expand(data, is_quoted, ft_getenv(data, var, true)))
		return (MALLOC_FAIL);
	return (EXIT_SUCCESS);
}

int	expand(char *content, t_var *data, bool is_quoted)
{
	char	*first;
	char	*var_name;
	int		len;

	len = 0;
	while (content[len])
	{
		first = get_word(content + len, "$");
		if (!first)
			return (ft_free(&content), MALLOC_FAIL);
		len += ft_strlen(first);
		if (*first && add_chunk(data, first, data->to_join++) == MALLOC_FAIL)
			return (ft_free(&content), ft_free(&first), MALLOC_FAIL);
		ft_free(&first);
		if (ft_strchr(content + len, '$') == NULL)
			break ;
		var_name = get_var_name(ft_strchr(content + len, '$') + 1);
		if (!var_name)
			return (ft_free(&content), MALLOC_FAIL);
		len += ft_strlen(var_name) + 1;
		if (expand_var(data, var_name, is_quoted) == MALLOC_FAIL)
			return (ft_free(&var_name), ft_free(&content), MALLOC_FAIL);
		ft_free(&var_name);
	}
	return (ft_free(&content), EXIT_SUCCESS);
}

int	add_chunk(t_var *data, char *str, bool to_join)
{
	t_token	**list;
	t_token	*new;

	if (data->is_heredoc == true)
		list = &data->heredoc_input;
	else
		list = &data->command_list;
	if (str && list)
	{
		if (to_join && *list)
			join_to_last(*list, str);
		else
		{
			new = create_new_token(str, WORD);
			if (!new)
				return (MALLOC_FAIL);
			add_token_to_back(list, new);
		}
	}
	return (EXIT_SUCCESS);
}
