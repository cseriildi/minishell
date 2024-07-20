/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parsing_table.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:08:11 by pvass             #+#    #+#             */
/*   Updated: 2024/07/20 16:00:25 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_table	*init_table(int fd)
{
	t_table	*table;
	t_table	*temp;
	char	**args;
	char	*line;

	table = NULL;
	line = get_next_line(fd);
	free(line);
	line = get_next_line(fd);
	if (line == NULL)
		return (NULL);
	while (line != NULL)
	{
		args = ft_split(line, 9);
		free(line);
		if (args == NULL)
			return (table_clear(&table), NULL);
		temp = table_new(args);
		free_tab(args);
		if (temp == NULL)
			return (table_clear(&table), NULL);
		table_add_back(&table, temp);
		line = get_next_line(fd);
	}
	free(line);
	return (table);
}

t_table	*create_table(void)
{
	t_table	*table;
	int		fd;

	fd = open(PARSING_TABLE, O_RDONLY);
	if (fd < 0)
		return (NULL);
	table = init_table(fd);
	close(fd);
	return (table);
}

void	print_p_table(t_table *parsing_table)
{
	//t_table *temp;
	//temp = parsing_table;
	printf("TABLE: \n");
	while (parsing_table != NULL)
	{
		printf("%d	%d	%d	%d	%d\n", parsing_table->state, parsing_table->event, parsing_table->action, parsing_table->next_s, parsing_table->nb_reduce);
		parsing_table = parsing_table->next;
	}
	//table_clear(&temp);
	//return (0);
}

/* int main ()
{
	t_table *t;

	t = create_table();
	return (0);
} */