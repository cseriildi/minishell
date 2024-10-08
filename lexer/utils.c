/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:28:15 by icseri            #+#    #+#             */
/*   Updated: 2024/09/25 13:58:49 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
