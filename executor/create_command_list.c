/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 11:22:46 by pvass             #+#    #+#             */
/*   Updated: 2024/10/25 11:59:59 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	create_cmd_list_one(t_var *data, t_exec *exec)
{
	int		i;
	t_exec	*temp;
	t_exec	*next;

	temp = exec;
	i = 0;
	while (temp != NULL && temp->type == WORD)
	{
		next = temp->down;
		fix_content(data, temp, true);
		temp = next;
	}
	temp = exec;
	while (temp != NULL)
	{
		if (temp->type == WORD)
			i++;
		temp = temp->down;
	}
	data->cmd_list = malloc(sizeof(char *) * (i + 1));
	if (!data->cmd_list)
		malloc_failed(data);
}

int	create_cmd_list_util(t_var *data, t_exec **temp, int i)
{
	char	*cmd;

	cmd = ft_strdup((*temp)->data);
	if (!cmd)
		malloc_failed(data);
	if (*cmd == '$' && *(*temp)->data == '\0')
	{
		free(cmd);
		*temp = (*temp)->down;
		return (i);
	}
	data->cmd_list[i] = cmd;
	i++;
	return (i);
}

void	create_cmd_list(t_var *data, t_exec *exec)
{
	int		i;
	t_exec	*temp;

	free_array(&data->cmd_list);
	if (exec->type != WORD)
		return ;
	create_cmd_list_one(data, exec);
	temp = exec;
	i = 0;
	while (temp != NULL)
	{
		if (temp->type == WORD)
			i = create_cmd_list_util(data, &temp, i);
		temp = temp->down;
	}
	data->cmd_list[i] = NULL;
}
