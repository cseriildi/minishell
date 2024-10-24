/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:24:05 by icseri            #+#    #+#             */
/*   Updated: 2024/10/24 18:37:33 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	fix_exec(t_var *data, t_exec *exec)
{
	t_exec	*temp;
	t_exec	*new;
	t_token	*curr;
	char	*tmp;

	curr = data->command_list;
	temp = exec;
	if (!curr)
	{
		if (temp->type == WORD)
			temp->type = NONE;
		else
			ft_free(&temp->data);
		return ;
	}
	if (!curr->content)
	{
		if (temp->type == WORD)
			temp->type = NONE;
		else
			ft_free(&temp->data);
	}
	else
	{
		tmp = ft_strdup(curr->content);
		if (tmp == NULL)
			malloc_failed(data);
		ft_free(&temp->data);
		temp->data = tmp;
	}
	curr = curr->next;
	while (curr)
	{
		if ((!curr->next && !*curr->content) || !curr->content)
		{
			curr = curr->next;
			continue ;
		}
		new = create_exec_node(curr->content);
		if (!new)
			malloc_failed(data);
		new->type = temp->type;
		new->down = temp->down;
		temp->down = new;
		temp = temp->down;
		curr = curr->next;
	}
}

t_exec *create_exec_node(char *content)
{
	t_stack *res;
	t_exec *exec;
	
	res = malloc(sizeof(t_stack));
	if (!res)
		return (NULL);
	res->data = ft_strdup(content);
	if (!res->data)
		return NULL;
	res->state = 0;
	res->type = WORD;
	res->next = NULL;
	exec = exec_new(&res);
	if (!exec)
	{
		free(res);
		return (NULL);
	}
	return (exec);
}

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
		if (seq->data[index] == '\'' || seq->data[index] == '\"')
			chunk = ft_substr(seq->data, index + 1, len - 2);
		else
			chunk = ft_substr(seq->data, index, len);
		if (chunk == NULL)
			malloc_failed(data);
		if (expandable  && seq->data[index] != '\'' && !(len == 2 && seq->data[index] == '\"'))
		{
			if(expand(chunk, data, seq->data[index] == '\"') == MALLOC_FAIL)
				malloc_failed(data);
		}
		else
		{
			if(add_chunk(data, chunk, data->to_join++) == MALLOC_FAIL)
				return (ft_free(&chunk), malloc_failed(data));
			ft_free(&chunk);
		}
		index += len;
	}
	fix_exec(data, seq);
	free_tokens(&data->command_list);
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
void	join_to_last_token(t_var *data, char *to_join);
int	expand(char *content, t_var *data, bool is_quoted)
{
	char	*first;
	char	*var_name;
	char	**expanded_var;
	char	*var;
	char	*number;
	int		i;
	int		len;
	char	*var_from_env;
	
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
		var = ft_strchr(content + len, '$');
		if (!var)
			break ;
		var_name = get_var_name(ft_strchr(content + len, '$') + 1);
		if (!var_name)
			return (ft_free(&content), MALLOC_FAIL);
		len += ft_strlen(var_name) + 1;
		if (!*var_name)
		{
			ft_free(&var_name);
			if (add_chunk(data, "$", data->to_join++) == MALLOC_FAIL)
				return (ft_free(&content), MALLOC_FAIL);
		}
		else if (*var_name == '?')
		{
			ft_free(&var_name);
			number = ft_itoa(data->exit_code);
			if (!number)
				return (ft_free(&content), MALLOC_FAIL);
			if (add_chunk(data, number, data->to_join++) == MALLOC_FAIL)
				return (ft_free(&content), ft_free(&number), MALLOC_FAIL);
			ft_free(&number);
		}
		else if (*var_name == ' ')
		{
			ft_free(&var_name);
			if (add_chunk(data, "$ ", data->to_join++) == MALLOC_FAIL)
				return (ft_free(&content), MALLOC_FAIL);
		}
		else
		{
			var_from_env = ft_getenv(data, var_name, true);
			ft_free(&var_name);
			if (*var_from_env == '\0' || is_quoted)
			{
				if (is_quoted && add_chunk(data, var_from_env, data->to_join++) == MALLOC_FAIL)
					return (ft_free(&content), MALLOC_FAIL);
				continue ;
			}
			expanded_var = easy_split(var_from_env, " \t\n\v\f\r");
			if (!expanded_var)
				return (ft_free(&content), MALLOC_FAIL);
			data->to_join = ft_strchr(" \t\n\v\f\r", *var_from_env) == NULL;
			i = -1;
			while (expanded_var[++i])
			{
				if (add_chunk(data, expanded_var[i], data->to_join) == MALLOC_FAIL)
					return (ft_free(&content), free_array(&expanded_var), MALLOC_FAIL);
				data->to_join = 0;
			}
			data->to_join = ft_strchr(" \t\n\v\f\r", var_from_env[ft_strlen(var_from_env) - 1]) == NULL;
			free_array(&expanded_var);
		}
	}
	ft_free(&content);
	return (EXIT_SUCCESS);
}

char *get_var_name(char *str)
{
	int		i;

	if (str[0] == '?')
		return (ft_substr(str, 0, 1));
	if (str[0] == ' ') //what about other whitespaces
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


int	add_chunk(t_var *data, char *str, bool to_join)
{
	t_token	*curr;
	t_token **list;
	t_token	*new;
	char	*tmp;

	if (!str /* || !*str */) //new: || !*str
		return (EXIT_SUCCESS);
	if (data->is_heredoc == true)
		list = &data->heredoc_input;
	else
		list = &data->command_list;
	if (list)
	{
		curr = *list;
		if (to_join && *list)
		{
			while (curr->next)
				curr = curr->next;
			tmp = ft_strjoin(curr->content, str);
			if (!tmp)
				return (MALLOC_FAIL);
			ft_free(&curr->content);
			curr->content = tmp;
		}
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
