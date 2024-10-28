/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_exist.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:05:42 by pvass             #+#    #+#             */
/*   Updated: 2024/10/28 11:46:18 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	redirs_exist_2(t_var *data, t_exec *exec, t_exec *temp2)
{
	if ((temp2->type != RED_IN || exec->type == WORD)
		&& is_directory(temp2->data) == true)
	{
		data->exit_code = 1;
		print_error(3, "minishell: ", temp2->data, ": Is a directory");
		return (0);
	}
	if (temp2->type != RED_IN && !good_redir_path(temp2->data, data))
	{
		data->exit_code = 1;
		print_error(3, "minishell: ",
			temp2->data, ": No such file or directory");
		return (0);
	}
	if ((temp2->type == RED_IN && access(temp2->data, F_OK) == -1)
		|| !*temp2->data)
	{
		data->exit_code = 1;
		print_error(3, "minishell: ",
			temp2->data, ": No such file or directory");
		return (0);
	}
	return (1);
}

bool	redirs_exist(t_var *data, t_exec *exec)
{
	t_exec	*temp2;
	char	*filename;
	int		count;

	temp2 = exec;
	while (temp2 != NULL)
	{
		if (is_in_out_app(temp2) == 1)
		{
			count = count_exec(temp2);
			filename = ft_strdup(temp2->data);
			fix_content(data, temp2, true);
			if (!temp2->data || count != count_exec(temp2))
			{
				data->exit_code = 1;
				print_error(3, "minishell: ", filename, ": ambiguous redirect");
				return (free(filename), 0);
			}
			free(filename);
			if (redirs_exist_2(data, exec, temp2) == 0)
				return (0);
		}
		temp2 = temp2->down;
	}
	return (1);
}
