/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:24:05 by icseri            #+#    #+#             */
/*   Updated: 2024/10/02 15:34:30 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*fix_content(t_var *data, char *str, bool expandable)
{
	char	*expanded;
	int		index;
	int		len;
	char	*chunk;
	char	*tmp;

	index = 0;
	len = 0;
	expanded = ft_strdup("");
	while (str[index])
	{
		len = get_chunk_size(str + index);
		if (str[index] == '\'' || str[index] == '\"')
			chunk = ft_substr(str, index + 1, len - 2);
		else
			chunk = ft_substr(str, index, len);
		if (expandable && str[index] != '\'' && chunk)
			chunk = expand(chunk, data);
		if (chunk == NULL)
		{
			free(expanded);
			print_error(1, "minishell: malloc failed");
			safe_exit(data, MALLOC_FAIL);
		}
		tmp = ft_strjoin(expanded, chunk);
		free(expanded);
		free(chunk);
		expanded = tmp;
		if (expanded == NULL)
		{
			print_error(1, "minishell: malloc failed");
			safe_exit(data, MALLOC_FAIL);
		}
		index += len;
	}
	return (expanded);
}

int	get_chunk_size(char *str)
{
	int		i;
	int		j;
	char	*separator;
	bool	is_quoted;

	if (*str == '\"')
		separator = "\"";
	else if (*str == '\'')
		separator = "\'";
	else
		separator = "|><\'\"";
	is_quoted = (*str == '\'' || *str == '\"');
	i = is_quoted;
	while (str[i])
	{
		j = 0;
		while (separator[j] && separator[j] != str[i])
			j++;
		if (separator[j] && separator[j] == str[i])
			return (i + is_quoted);
		i++;
	}
	return (i + is_quoted);
}
char *get_var_name(char *str);

char	*expand(char *content, t_var *data)
{
	char	*first;
	char	*var_name;
	char	*last;
	char	*tmp;
	char	*expanded_var;
	char	*var;

	var = ft_strchr(content, '$');
	while (var && var[1] && ft_strchr(" \t\n\v\f\r|><\'\"", var[1]) == NULL)
	{
		first = get_word(content, "$");
		if (!first)
			return (ft_free(&content), NULL);
		var_name = get_var_name(var + 1);
		if (!var_name)
			return (ft_free(&content), ft_free(&first), NULL);
		if (*var_name == '?')
			expanded_var = ft_itoa(data->exit_code);
		else
			expanded_var = ft_strdup(safe_getenv(data, var_name));
		if (!expanded_var)
			return (ft_free(&content), free(first), free(var_name), NULL);
		tmp = ft_strjoin(first, expanded_var);
		ft_free(&expanded_var);
		if (!tmp)
			return (ft_free(&content), free(first), free(var_name), NULL);
		if (*var_name == '?')
			last = ft_strdup(content + ft_strlen(first) + 2);
		else
			last = ft_strdup(content
					+ ft_strlen(first) + ft_strlen(var_name) + 1);
		ft_free(&content);
		ft_free(&first);
		ft_free(&var_name);
		if (!last)
			return (ft_free(&tmp), NULL);
		content = ft_strjoin(tmp, last);
		ft_free(&tmp);
		ft_free(&last);
		if (!content)
			return (NULL);
		var = ft_strchr(content, '$');
	}
	return (content);
}

char *get_var_name(char *str)
{
	int		i;

	if (str[0] == '?')
		return (ft_substr(str, 0, 1));
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if ((i == 0 && (!ft_isalpha(str[i]) && str[i] != '_'))
			|| (!ft_isalnum(str[i]) && str[i] != '_'))
			break ;
		i++;
	}
	return (ft_substr(str, 0, i));
}
