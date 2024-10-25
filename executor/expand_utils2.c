/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:54:12 by icseri            #+#    #+#             */
/*   Updated: 2024/10/25 12:58:14 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	fix_head(t_var *data, t_exec *exec)
{
	char	*tmp;

	if (!data->command_list || !data->command_list->content)
	{
		if (exec->type == WORD)
			exec->type = NONE;
		else
			ft_free(&exec->data);
	}
	else
	{
		tmp = ft_strdup(data->command_list->content);
		if (tmp == NULL)
			malloc_failed(data);
		ft_free(&exec->data);
		exec->data = tmp;
	}
	return (data->command_list != NULL);
}

void	fix_exec(t_var *data, t_exec *exec)
{
	t_exec	*temp;
	t_exec	*new;
	t_token	*curr;

	if (fix_head(data, exec) == false)
		return ;
	temp = exec;
	curr = data->command_list->next;
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
	free_tokens(&data->command_list);
}

int	join_to_last(t_token *curr, char *str)
{
	char	*tmp;

	while (curr->next)
		curr = curr->next;
	tmp = ft_strjoin(curr->content, str);
	if (!tmp)
		return (MALLOC_FAIL);
	ft_free(&curr->content);
	curr->content = tmp;
	return (EXIT_SUCCESS);
}

char	*get_chunk(t_var *data, char *str, int index, int len)
{
	char	*chunk;

	if (str[index] == '\'' || str[index] == '\"')
		chunk = ft_substr(str, index + 1, len - 2);
	else
		chunk = ft_substr(str, index, len);
	if (chunk == NULL)
		malloc_failed(data);
	return (chunk);
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
