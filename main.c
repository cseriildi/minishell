/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/09/23 13:04:31 by icseri           ###   ########.fr       */
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
	data->p_table = NULL;
	data->line = NULL;
	data->exit_code = 0;
	data->subshell_level = 1;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		safe_exit(data, MALLOC_FAIL);
	data->promt = NULL;
	init_env(data);
	//data->p_table = create_table();
}

int	main(int argc, char **argv)
{
	t_var	*data;

	(void)argv;
	if (argc != 1)
		return (print_error(2, "too many arguments, rerun like ./minishell"), ERROR_MISUSE);
	data = malloc(sizeof(t_var));
	if (!data)
		return (MALLOC_FAIL);
	init(data);
	while (true)
	{
		get_promt(data);
		data->line = readline(data->promt);
		if (!data->line)
			safe_exit(data, READLINE_FAIL);
		if (*data->line)
			add_history(data->line);
		lexer(data);
		printf("kurva\n");
		parse(data);
		printf("kurva2\n");
		execute(data);
		//free_tokens(data);
	}
	safe_exit(data, data->exit_code);
}
