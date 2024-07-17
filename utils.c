/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:28:15 by icseri            #+#    #+#             */
/*   Updated: 2024/07/17 12:38:01 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_word(char *text, char *separator)
{
	char	*word;
	int		sep_len;
	int		i;

	sep_len = ft_strlen(separator);
	i = 0;
	while (text[i] && ft_strncmp(text + i, separator, sep_len))
		i++;
	if (i == 0)
	{
		printf("not good\n");
		exit(3);
	}
	word = ft_substr(text, 0, i);
	return (word);
}
