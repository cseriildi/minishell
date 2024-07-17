/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:33:53 by icseri            #+#    #+#             */
/*   Updated: 2024/07/17 14:18:58 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		printf("%s\n", tokens[i]->content);
		printf("%d\n", tokens[i]->type);
		i++;
	}
}
/* 
void	debug_print_line(const char *line)
{
	printf("Debug: ");
	for (size_t i = 0; i < strlen(line) + 1; i++) { // +1 to include the null terminator
		if (line[i] == '\0') {
			printf("\\0");
		} else {
			putchar(line[i]);
		}
	}
	putchar('\n');
}
 */

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
			//debug_print_line(line);
			if (ft_strncmp(line, "exit", 5) == 0)
			{
				free(line);
				break ;
			}
			printf("[%s]\n", line);
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
