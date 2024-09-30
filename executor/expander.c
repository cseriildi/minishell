/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:24:05 by icseri            #+#    #+#             */
/*   Updated: 2024/09/30 14:39:57 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
char	*fix_content(char *content, t_var *data);
char	*get_word(char *text, char *separator);
int	get_chunk_size(char *str);

char	*expander(t_var *data, char *str)
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
		if (*str == '\'' || *str == '\"')
			chunk = ft_substr(str, index + 1, len - 1);
		else
			chunk = ft_substr(str, index, len);
		if (chunk == NULL)
		{
			free(expanded);
			print_error(1, "minishell: malloc failed");	
			safe_exit(data, MALLOC_FAIL);
		}
		if (*str != '\'')
			chunk = fix_content(chunk, data);
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

	if (*str == '\"')
		separator  = "\"";
	else if (*str == '\'')
		separator  = "\'";
	else
		separator  = "|><\'\"";
	i = 1;
	while (str[i])
	{
		j = 0;
		while (separator[j] && separator[j] != str[i])
			j++;
		if (separator[j] && separator[j] == str[i])
			return (i++);
		i++;
	}
	return (i);
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
			print_error(1, "minishell: malloc failed");	
			safe_exit(data, MALLOC_FAIL);
		}
		var_name = get_word(ft_strchr(content, '$') + 1, " $|><\'\"");
		if (!var_name)
		{
			ft_free(&content);
			ft_free(&first);
			print_error(1, "minishell: malloc failed");	
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
			print_error(1, "minishell: malloc failed");	
			safe_exit(data, MALLOC_FAIL);
		}
		tmp = ft_strjoin(first, expanded_var);
		ft_free(&expanded_var);
		if (!tmp)
		{
			ft_free(&content);
			ft_free(&first);
			ft_free(&var_name);
			print_error(1, "minishell: malloc failed");	
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
			print_error(1, "minishell: malloc failed");	
			safe_exit(data, MALLOC_FAIL);
		}
		content = ft_strjoin(tmp, last);
		ft_free(&tmp);
		ft_free(&last);
		if (!content)
		{
			print_error(1, "minishell: malloc failed");	
			safe_exit(data, MALLOC_FAIL);
		}
	}
	return (content);
}
