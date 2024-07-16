/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:01:43 by icseri            #+#    #+#             */
/*   Updated: 2024/07/16 18:18:19 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_assignment(char *var_name)
{
	char	*content;

	content = ft_strchr(gettenv(var_name), '=') + 1;
	return (content);
}

char	*assignment(char *text, t_token	**tokens)
{
	char	*content;
	char	*var_name;
	t_token	*new_token;

	var_name = get_word(text, " ");
	if (!var_name)
		return (NULL);
	content = get_assignment(var_name);
	new_token = create_new_token(content, ASSIGNMENT_WORD);
	add_token_to_back(tokens, new_token);
	return (text + ft_strlen(var_name));
}

char	*single_quote(char *text, t_token	**tokens)
{
	char	*content;
	t_token	*new_token;

	if (ft_strchr(text, '\'') == NULL)
		return (NULL);
	content = get_word(text, "\'");
	if (!content)
		return (NULL);
	new_token = create_new_token(content, WORD);
	add_token_to_back(tokens, new_token);
	return (text + ft_strlen(content));
}

char	*double_quote(char *text, t_token	**tokens)
{
	char	*content;
	t_token	*new_token;
	int		i;

	if (ft_strchr(text, '\"') == NULL)
		return (NULL);
	content = get_word(text, "\"");
	if (!content)
		return (NULL);
	i = 0;
	while (ft_strchr(content, '$') != NULL)
	{
		while (content[i] != '$')
			i++;
	}
	new_token = create_new_token(content, WORD);
	add_token_to_back(tokens, new_token);
	return (text + ft_strlen(content));
}

char	*get_next_token(char *text, t_token **tokens)
{
	if (*text == '$')
		text = assignment(text + 1, tokens);
	/* 
	else if (*text == '\"')
	{
		
	}
	else if (*text == '\'')
	{
		get_word(text, "\'")
	}
	else if (*text == '|')
	{
		
	}
	else if (ft_strncmp(text, "<<", 2) == 0)
	{
		
	}
	else if (ft_strncmp(text, ">>", 2) == 0)
	{
		
	}
	else if (*text == '<')
	{
		
	}
	else if (*text == '>')
	{
		
	} */
	return (text);
}

t_token	**lexer(char *text)
{
	t_token	**tokens;

	tokens =  malloc(sizeof(t_token *));
	if (!tokens)
		return (NULL);
	while (*text)
		text = get_next_token(text, tokens);
	return (tokens);
}
