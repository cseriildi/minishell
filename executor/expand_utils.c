/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:30:59 by icseri            #+#    #+#             */
/*   Updated: 2024/10/25 12:57:59 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	get_delim_len(char *str, char *delim)
{
	int		i;

	i = 0;
	while (str[i] && ft_strchr(delim, str[i]))
		i++;
	return (i);
}

int	get_len(char *str, char *delim)
{
	int		i;

	i = 0;
	while (str[i] && !ft_strchr(delim, str[i]))
		i++;
	return (i);
}

int	count_words(char *str, char *delim)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (ft_strchr(delim, str[i]))
			i += get_delim_len(str + i, delim);
		else
		{
			count++;
			i += get_len(str + i, delim);
		}
	}
	return (count);
}

char	**easy_split(char *str, char *delim)
{
	int		i;
	int		len;
	char	**words;
	int		count;

	if (!str || !delim)
		return (NULL);
	words = ft_calloc(sizeof(char *), count_words(str, delim) + 1);
	if (!words)
		return (NULL);
	i = 0;
	count = 0;
	while (str[i])
	{
		i += get_delim_len(str + i, delim);
		len = get_len(str + i, delim);
		if (len == 0)
			break ;
		words[count] = ft_substr(str, i, len);
		if (!words[count])
			return (free_array(&words), NULL);
		count++;
		i += len;
	}
	return (words);
}

char	*get_var_name(char *str)
{
	int		i;

	if (str[0] == '?')
		return (ft_substr(str, 0, 1));
	if (str[0] == ' ')
		return (ft_substr(str, 0, 1));
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if ((i == 0 && (!ft_isalpha(str[i]) && str[i] != '_'))
			|| (!ft_isalnum(str[i]) && str[i] != '_'))
			break ;
		i++;
	}
	return (ft_substr(str, 0, i));
}
