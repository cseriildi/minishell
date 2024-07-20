/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/07/20 11:15:32 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token **tokens)
{
	t_token	*head;
	t_token	*current;

	head = *tokens;
	current = head;
	while (current != NULL)
	{
		printf("%s\n", current->content);
		printf("%d\n", current->type);
		current = current->next;
	}
}

void	init(t_var *data)
{
	data->tokens = NULL;
	data->line = NULL;
	data->exit_code = 0;
}

int	main(int argc, char **argv)
{
	t_var	*data;

	(void)argv;
	if (argc != 1)
		return (1);
	data = malloc(sizeof(t_var));
	if (!data)
		return (MALLOC_FAIL);
	init(data);
	while (true)
	{
		data->line = readline("> ");
		if (!data->line)
			safe_exit(data, READLINE_FAIL);
		if (ft_strncmp(data->line, "exit", 5) == 0)
			safe_exit(data, EXIT_SUCCESS);
		if (*data->line)
			add_history(data->line);
		data->pid = fork();
		if (data->pid == -1)
			safe_exit(data, FORK_FAIL);
		else if (data->pid == 0)
		{
			lexer(data);
			check_brackets(data);
			print_tokens(data->tokens);
			safe_exit(data, EXIT_SUCCESS);
		}
		else
		{
			waitpid(data->pid, &data->exit_status, 0);
			if (WIFEXITED(data->exit_status))
				data->exit_code = WEXITSTATUS(data->exit_status);
			continue ;
		}
	}
	safe_exit(data, EXIT_SUCCESS);
}
