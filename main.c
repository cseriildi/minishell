/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/07/17 16:36:03 by icseri           ###   ########.fr       */
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
 	while (1)
	{
		line = readline(">");
		if (line)
		{
			if (ft_strncmp(line, "exit", 5) == 0)
			{
				free(line);
				break ;
			}
			if (*line)
				add_history(line);
			tokens = lexer(line);
			if (tokens)
			{
				print_tokens(tokens);
				free_tokens(tokens);
			}
			free(line);
		}
		else
		{
			perror("Error reading input");
			return (1);
		}
	}
	return (0);
}
