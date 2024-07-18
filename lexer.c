/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:01:43 by icseri            #+#    #+#             */
/*   Updated: 2024/07/18 18:15:32 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_assignment(char *var_name)
{
	char	*content;

	content = getenv(var_name);
	if (content == NULL)
		exit (1);
	return (++content);
}

char	*assignment(char *text, t_token	**tokens)
{
	char	*content;
	char	*var_name;
	t_token	*new_token;
	int		i;

	var_name = get_word(text, " ()$|><\'\"&");
	if (!var_name)
		exit (1);
	content = get_assignment(var_name);
	new_token = create_new_token(content, ASSIGNMENT_WORD);
	add_token_to_back(tokens, new_token);
	i = ft_strlen(var_name);
	return (text + i);
}

char	*single_quote(char *text, t_token	**tokens)
{
	char	*content;
	t_token	*new_token;
	int		i;

	if (ft_strchr(text, '\'') == NULL)
		exit (1);
	content = get_word(text, "\'");
	if (!content)
		exit (1);
	new_token = create_new_token(content, WORD);
	add_token_to_back(tokens, new_token);
	i = ft_strlen(content) + 1;
	return (text + i);
}

char	*fix_content(char *content)
{
	char	*first;
	char	*var_name;
	char	*last;
	char	*tmp;

	first = get_word(content, "$");
	if (!first)
		exit (1);
	var_name = get_word(ft_strchr(content, '$'), " ()$|><\'\"&");
	if (!var_name)
		exit (1);
	tmp = ft_strjoin(first, get_assignment(var_name + 1));
	if (!tmp)
		exit (1);
	last = ft_strdup(content + ft_strlen(first) + ft_strlen(var_name));
	free(content);
	free(first);
	free(var_name);
	content = ft_strjoin(tmp, last);
	free(tmp);
	free(last);
	if (!content)
		exit (1);
	return (content);
}

char	*double_quote(char *text, t_token **tokens)
{
	char	*content;
	t_token	*new_token;
	int		i;

	if (ft_strchr(text, '\"') == NULL)
		exit (1);
	content = get_word(text, "\"");
	if (!content)
		exit (1);
	i = ft_strlen(content) + 1;
	while (ft_strchr(content, '$') != NULL)
		content = fix_content(content);
	new_token = create_new_token(content, WORD);
	add_token_to_back(tokens, new_token);
	return (text + i);
}

char	*redirection(char *text, t_token **tokens)
{
	char	*filename;
	t_token	*new_token;
	int		i;

	i = 0;
	while (text[i] && ft_isalpha(text[i]) != 1)
		i++;
	filename = get_word(text + i, " ()$|><\'\"&");
	printf("file: %s\n", filename);
	if (!filename || !*filename)
		exit (1);
	if (ft_strncmp(text, ">>", 2) == 0)
		new_token = create_new_token(filename, APPEND);
	else
		new_token = create_new_token(filename, RED_OUT);
	add_token_to_back(tokens, new_token);
	i += ft_strlen(filename);
	return (text + i);
}

char	*input(char *text, t_token **tokens)
{
	char	*filename;
	t_token	*new_token;
	int		i;

	i = 0;
	while (text[i] && ft_isalpha(text[i]) != 1)
		i++;
	filename = get_word(text + i, " ()$|><\'\"&");
	if (!filename|| !*filename)
		exit (1);
	if (ft_strncmp(text, "<<", 2) == 0)
		new_token = create_new_token(filename, HERE_DOC);
	else
		new_token = create_new_token(filename, RED_IN);
	add_token_to_back(tokens, new_token);
	i += ft_strlen(filename);
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
	return (text + i);
}

char	*and_sign(char *text, t_token **tokens)
{
	t_token	*new_token;
	int		i;

	if (ft_strncmp(text, "&&", 2) != 0)
		exit (1);
	new_token = create_new_token("&&", AND);
	add_token_to_back(tokens, new_token);
	i = 2;
	return (text + i);
}

char	*word(char *text, t_token **tokens)
{
	t_token	*new_token;
	char	*content;
	int		i;

	i = 0;
	content = get_word(text, " ()$|><\'\"&");
	if (content == NULL)
		exit (1);
	new_token = create_new_token(content, WORD);
	add_token_to_back(tokens, new_token);
	i += ft_strlen(content);
	return (text + i);
}

char	*get_next_token(char *text, t_token **tokens)
{
	while (*text && *text == ' ')
		text++;
	if (!text)
		return ;
	if (*text == '$')
		text = assignment(++text, tokens);
	else if (*text == '\"')
		text = double_quote(++text, tokens);
	else if (*text == '\'')
		text = single_quote(++text, tokens);
	else if (*text == '|')
		text = pipes(text, tokens);
	else if (*text == '<')
		text = input(text, tokens);
	else if (*text == '>')
		text = redirection(text, tokens);
	else if (*text == '&')
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

	if (!text)
		return (NULL);
	tokens = malloc(sizeof(t_token *));
	if (!tokens)
		return (NULL);
	*tokens = NULL;
	while (*text)
		text = get_next_token(text, tokens);
	add_token_to_back(tokens, create_new_token("END", END));
	return (tokens);
}
