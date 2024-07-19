/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:28:15 by icseri            #+#    #+#             */
/*   Updated: 2024/07/19 17:42:49 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_word(char *text, char *separator)
{
	char	*word;
	int		i;
	int		j;

	i = 0;
	while (text[i])
	{
		j = 0;
		while (separator[j] && separator[j] != text[i])
			j++;
		if (separator[j] && separator[j] == text[i])
			break ;
		i++;
	}
	word = ft_substr(text, 0, i);
	return (word);
}

void	safe_exit(t_var *data, int exit_code)
{
	if (data)
	{
		if (data->line)
			free(data->line);
		if (data->tokens)
			free_tokens(data->tokens);
		free(data);
	}
	exit(exit_code);
}
