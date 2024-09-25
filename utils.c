/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:50:26 by icseri            #+#    #+#             */
/*   Updated: 2024/09/25 14:16:42 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_var *data)
{
	t_token	*current;
	t_token	*next;

	if (data->tokens)
	{
		current = data->tokens;
		while (current != NULL)
		{
			next = current->next;
			ft_free(&current->content);
			free(current);
			current = next;
		}
		data->tokens = NULL;
	}
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr && arr[i])
			ft_free(&arr[i++]);
		free(arr);
	}
	arr = NULL;
}

void	safe_exit(t_var *data, int exit_code)
{
	if (data)
	{
		free_tokens(data);
		ft_free(&data->pwd);
		ft_free(&data->promt);
		ft_free(&data->line);
		free_array(data->env);
		free_array(data->cmd_list);
		free_exec_all(&(data->exec));
		free_table(&(data->p_table));
		free(data);
	}
	exit(exit_code);
}

void	get_promt(t_var *data)
{
	char	*home;
	char	*tmp;

	home = ft_getenv(data, "HOME");
	if (home != NULL
		&& ft_strncmp(data->pwd, home, ft_strlen(home)) == 0)
	{
		tmp = ft_strjoin("~", data->pwd + ft_strlen(home));
		if (!tmp)
			safe_exit(data, MALLOC_FAIL);
		data->promt = ft_strjoin(tmp, "$ ");
		free(tmp);
	}
	else
		data->promt = ft_strjoin(data->pwd, "$ ");
	if (!data->promt)
		safe_exit(data, MALLOC_FAIL);
}

void	print_error(int count, ...)
{
	va_list	args;
	char	*str;

	va_start(args, count);
	while (count--)
	{
		str = va_arg(args, char *);
		if (str)
			ft_putstr_fd(str, STDERR_FILENO);
	}
	va_end(args);
	ft_putstr_fd("\n", STDERR_FILENO);
}
