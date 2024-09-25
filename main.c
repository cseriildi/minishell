/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/09/25 12:23:02 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signals signals;

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
	data->exec = NULL;
	data->line = NULL;
	data->exit_code = 0;
	data->subshell_level = 1;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		safe_exit(data, MALLOC_FAIL);
	data->promt = NULL;
	data->p_table = create_table();
	if (data->p_table == NULL)
		safe_exit(data, MALLOC_FAIL);
	init_env(data);
}

/* // Signal handler for Ctrl-C (SIGINT)
void handle_sigint(int sig) {
    (void)sig;
    printf("\nCtrl-C pressed\n");
}

// Signal handler for Ctrl-\ (SIGQUIT)
void handle_sigquit(int sig) {
    (void)sig;
    printf("Ctrl-\\ pressed\n");
}

// Function to handle Ctrl-D (EOF)
void handle_ctrl_d() {
    printf("Ctrl-D pressed\n");
    exit(0);  // Exit the shell as Ctrl-D should exit normally
} */

int	main(int argc, char **argv)
{
	t_var	*data;

	signals.child_pid = -1;
	signals.interactive = 1;
	(void)argv;
	if (argc != 1)
		return (print_error(2, "too many arguments, rerun like ./minishell"), ERROR_MISUSE);
	data = malloc(sizeof(t_var));
	if (!data)
		return (MALLOC_FAIL);
	init(data);
	init_signals();
	while (true)
	{
		get_promt(data);
		signals.interactive = 1;
		data->line = readline(data->promt);
		signals.interactive = 0;
		if (!data->line)
		{
			ft_printf("exit\n");
			safe_exit(data, READLINE_FAIL);
		}
		if (*data->line)
			add_history(data->line);
		lexer(data);
		parse(data);
		execute(data);
		/* if (!data->line) {
		    handle_ctrl_d();
		} */
		free_tokens(data);
		ft_free(&data->line);
		free_exec_all(&data->exec);
	}
	safe_exit(data, data->exit_code);
}
