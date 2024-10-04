/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:50:26 by icseri            #+#    #+#             */
/*   Updated: 2024/10/03 21:28:43 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **token)
{
	t_token	*current;
	t_token	*next;

	if (token && *token)
	{
		current = *token;
		while (current != NULL)
		{
			next = current->next;
			ft_free(&current->content);
			free(current);
			current = next;
		}
		*token = NULL;
	}
}
void	free_array(char ***arr)
{
	int	i;

	i = 0;
	if (arr && *arr)
	{
		while ((*arr)[i])
			ft_free(&(*arr)[i++]);
		free(*arr);
		*arr = NULL;
	}
}

void	free_all(t_var *data)
{
	if (data)
	{
		free_tokens(&data->tokens);
		free_tokens(&data->command_list);
		ft_free(&data->promt);
		ft_free(&data->line);
		delete_file(data, data->here_doc_filename);
		ft_free(&data->here_doc_filename);
		free_array(&data->cmd_list);
		free_exec_all(&(data->exec));
		safe_close(&data->pipe1_fd[0]);
		safe_close(&data->pipe1_fd[1]);
		safe_close(&data->pipe2_fd[0]);
		safe_close(&data->pipe2_fd[1]);
		safe_close(&data->stdout_copy);
	}
}

void	safe_exit(t_var *data, int exit_code)
{
	if (data)
	{
		ft_free(&data->pwd);
		free_array(&data->env);
		free_table(&(data->p_table));
		free_all(data);
		free(data);
	}
	exit(exit_code);
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
