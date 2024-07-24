/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cseriildii <cseriildii@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:32:22 by icseri            #+#    #+#             */
/*   Updated: 2024/07/24 10:11:50 by cseriildii       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	execute(t_var *data)
{
	printf("%s\n", ft_getenv(data, "PATH"));
	data->tree = malloc(sizeof(t_ast *));
	if (!data->tree)
		safe_exit(data, MALLOC_FAIL);
	*data->tree = malloc(sizeof(t_ast));
	(*data->tree)->child = malloc(sizeof(t_ast));
	(*data->tree)->child->next = malloc(sizeof(t_ast));
	(*data->tree)->data = "unset";
	(*data->tree)->child->data = "PATH";
	(*data->tree)->child->next->data = " ";
	if (builtin(data, *data->tree) == false)
	{
		data->pid = fork();
		if (data->pid == -1)
			safe_exit(data, FORK_FAIL);
		if (data->pid == 0)
		{
			if (execve("/bin/echo", (char *[]){"echo", "-n", "dfsd", NULL}, data->env) == -1)
				safe_exit(data, 42);
		}
		else
		{
			waitpid(data->pid, &data->exit_status, 0);
			if (WIFEXITED(data->exit_status))
				data->exit_code = WEXITSTATUS(data->exit_status);
			printf("exit code: %d\n", data->exit_code);
		}
	}
	printf("%s\n", ft_getenv(data, "PATH"));

}
