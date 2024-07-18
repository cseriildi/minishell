/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/07/18 17:20:57 by icseri           ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_token	**tokens;
	char	*line;

	(void)argv;
	if (argc != 1)
		return (1);
	while (true)
	{
		line = readline("> ");
		if (!line)
			return (perror("Error reading input"), 1);
		if (ft_strncmp(line, "exit", 5) == 0)
			return (free(line), 0);
		if (*line)
			add_history(line);
		tokens = lexer(line);
		free(line);
		if (!tokens)
			exit(1);
		print_tokens(tokens);
		free_tokens(tokens);
	}
	return (0);
}
