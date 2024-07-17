/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:01:43 by icseri            #+#    #+#             */
/*   Updated: 2024/07/17 14:20:10 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_assignment(char *var_name)
{
	char	*content;

	content = ft_strchr(getenv(var_name), '=');
	return (++content);
}

char	*assignment(char *text, t_token	**tokens)
{
	char	*content;
	char	*var_name;
	t_token	*new_token;
	int		i;

	var_name = get_word(text, " ");
	if (!var_name)
		return (NULL);
	content = get_assignment(var_name);
	new_token = create_new_token(content, ASSIGNMENT_WORD);
	add_token_to_back(tokens, new_token);
	i = ft_strlen(var_name);
	while (text[i] && text[i] == ' ')
		i++;
	return (text + i);
}

char	*single_quote(char *text, t_token	**tokens)
{
	char	*content;
	t_token	*new_token;
	int		i;

	if (ft_strchr(text, '\'') == NULL)
		return (NULL);
	content = get_word(text, "\'");
	if (!content)
		return (NULL);
	new_token = create_new_token(content, WORD);
	add_token_to_back(tokens, new_token);
	i = ft_strlen(content) + 1;
	while (text[i] && text[i] == ' ')
		i++;
	return (text + i);
}
/* 
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
		var_name = get_word(text, " ");
		if (!var_name)
			return (NULL);
		content = get_assignment(var_name);
	}
	new_token = create_new_token(content, WORD);
	add_token_to_back(tokens, new_token);
	return (text + ft_strlen(content));
} */

char	*redirection(char *text, t_token **tokens)
{
	char	*filename;
	t_token	*new_token;
	int		i;

	i = 0;
	while (ft_isalpha(text[i]) != 1)
		i++;
	filename = get_word(text + i, " ");
	if (!filename)
		return (NULL);
	if (ft_strncmp(text, ">>", 2) == 0)
		new_token = create_new_token(filename, APPEND);
	else
		new_token = create_new_token(filename, RED_OUT);
	add_token_to_back(tokens, new_token);
	i += ft_strlen(filename);
	while (text[i] && text[i] == ' ')
		i++;
	return (text + i);
}

char	*input(char *text, t_token **tokens)
{
	char	*filename;
	t_token	*new_token;
	int		i;

	i = 0;
	while (ft_isalpha(text[i]) != 1)
		i++;
	filename = get_word(text + i, " ");
	if (!filename)
		return (NULL);
	if (ft_strncmp(text, "<<", 2) == 0)
		new_token = create_new_token(filename, HERE_DOC);
	else
		new_token = create_new_token(filename, RED_IN);
	add_token_to_back(tokens, new_token);
	i += ft_strlen(filename);
	while (text[i] && text[i] == ' ')
		i++;
	return (text + i);
}

char	*pipes(char *text, t_token **tokens)
{
	t_token	*new_token;
	int		i;

	i = 0;
	while (text[i] == '|')
		i++;
	if (ft_strncmp(text, "||", 2) == 0)
		new_token = create_new_token("||", OR);
	else
		new_token = create_new_token("|", PIPE);
	add_token_to_back(tokens, new_token);
	while (text[i] && text[i] == ' ')
		i++;
	return (text + i);
}

char	*braces(char *text, t_token **tokens)
{
	t_token	*new_token;
	int		i;

	if (*text == '(')
		new_token = create_new_token("(", L_BRACKET);
	else
		new_token = create_new_token(")", R_BRACKET);
	add_token_to_back(tokens, new_token);
	i = 1;
	while (text[i] && text[i] == ' ')
		i++;
	return (text + i);
}

char	*and_sign(char *text, t_token **tokens)
{
	t_token	*new_token;
	int		i;

	new_token = create_new_token("&&", AND);
	add_token_to_back(tokens, new_token);
	i = 2;
	while (text[i] && text[i] == ' ')
		i++;
	return (text + i);
}

char	*word(char *text, t_token **tokens)
{
	t_token	*new_token;
	char	*content;
	int		i;

	i = 0;
	while (text[i] && text[i] == ' ')
		i++;
	content = get_word(text, " ");
	if (content == NULL)
		return (NULL);
	new_token = create_new_token(content, WORD);
	add_token_to_back(tokens, new_token);
	i += ft_strlen(content);
	while (text[i] && text[i] == ' ')
		i++;
	return (text + i);
}

char	*get_next_token(char *text, t_token **tokens)
{
	while (*text && *text == ' ')
		text++;
	if (*text == '$')
		text = assignment(++text, tokens);
	else if (*text == '\"')
		text = single_quote(++text, tokens);
	else if (*text == '\'')
		text = single_quote(++text, tokens);
	else if (*text == '|')
		text = pipes(text, tokens);
	else if (*text == '<')
		text = input(text, tokens);
	else if (*text == '>')
		text = redirection(text, tokens);
	else if (ft_strncmp(text, "&&", 2) == 0)
		text = and_sign(text, tokens);
	else if (*text == ')' || *text == '(')
		text = braces(text, tokens);
	else if (*text)
		text = word(text, tokens);
	return (text);
}

t_token	**lexer(char *text)
{
	t_token	**tokens;

	tokens = malloc(sizeof(t_token *));
	if (!tokens)
		return (NULL);
	while (*text)
		text = get_next_token(text, tokens);
	add_token_to_back(tokens, create_new_token("", END));
	return (tokens);
}
