/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:07:54 by pvass             #+#    #+#             */
/*   Updated: 2024/11/06 15:57:36 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ccc(char const *str, char c)
{
	int		i;
	int		counter;

	i = 0;
	counter = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			counter++;
		i++;
	}
	counter++;
	return (counter);
}

char	**count_w_create(char *str, char c)
{
	int		count;
	int		i;
	char	**words;
	int		len;

	count = ccc(str, c);
	words = ft_calloc(sizeof(char *) * (count + 1), 1);
	if (words == NULL)
		return (NULL);
	i = -1;
	while (++i < count)
	{
		len = 0;
		while (*str && *str != c)
		{
			str++;
			len++;
		}
		if (*str == c)
			str++;
		words[i] = ft_calloc(len + 1, 1);
		if (words[i] == NULL)
			return (ft_free(words), NULL);
	}
	return (words);
}

char	**path_split(char *str, char c)
{
	char	**words;
	int		i;
	int		j;

	if (str == NULL)
		return (NULL);
	words = count_w_create(str, c);
	if (words == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (*str)
	{
		if (*str != c)
			words[i][j++] = *str;
		else
		{
			words[i][j] = '\0';
			i++;
			j = 0;
		}
		str++;
	}
	words[i][j++] = *str;
	return (words[i + 1] = NULL, words);
}
