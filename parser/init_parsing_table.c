/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parsing_table.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvass <pvass@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:08:11 by pvass             #+#    #+#             */
/*   Updated: 2024/10/29 22:01:17 by pvass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_table	*init_table(char **longlonglong_tab)
{
	t_table	*table;
	t_table	*temp;
	char	**args;
	char	*line;
	int		i;

	table = NULL;
	line = longlonglong_tab[0];
	i = 1;
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
		line = longlonglong_tab[i];
		i++;
	}
	return (free(line), free(longlonglong_tab), table);
}

t_table	*create_table(void)
{
	t_table	*table;
	char	**longlonglong_tab;

	longlonglong_tab = ft_split(PARSE_LINE, '\n');
	if (longlonglong_tab == NULL)
		return (NULL);
	table = init_table(longlonglong_tab);
	return (table);
}

void	print_p_table(t_table *parsing_table)
{
	ft_printf("TABLE: \n");
	while (parsing_table != NULL)
	{
		ft_printf("%d	%d	%d	%d	%d\n", parsing_table->state,
			parsing_table->event, parsing_table->action,
			parsing_table->next_s, parsing_table->nb_reduce);
		parsing_table = parsing_table->next;
	}
}
