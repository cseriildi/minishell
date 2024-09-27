/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parsing_table.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:08:11 by pvass             #+#    #+#             */
/*   Updated: 2024/09/27 11:30:40 by pvass            ###   ########.fr       */
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
	if (line == NULL)
		return (NULL);
	free(line);
	line = get_next_line(fd);
	while (line != NULL)
	{
		args = ft_split(line, 9);
		if (args == NULL)
			return (free(line), free_table(&table), NULL);
		free(line);
		temp = table_new(args);
		free_tab(args);
		if (temp == NULL)
			return (table_clear(&table), NULL);
		table_add_back(&table, temp);
		line = get_next_line(fd);
	}
	return (free(line), table);
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
	printf("TABLE: \n");
	while (parsing_table != NULL)
	{
		printf("%d	%d	%d	%d	%d\n", parsing_table->state,
			parsing_table->event, parsing_table->action,
			parsing_table->next_s, parsing_table->nb_reduce);
		parsing_table = parsing_table->next;
	}
}
