/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:00:14 by icseri            #+#    #+#             */
/*   Updated: 2024/07/22 15:37:28 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*ft_getenv(t_var *data, char *var_name)
{
	t_env	*head;
	t_env	*curr;

	head = *data->env;
	curr = head;
	if (var_name && *var_name)
	{
		while (curr != NULL)
		{
			if (ft_strncmp(curr->key, var_name, ft_strlen(var_name) + 1) == 0)
				return (curr->content);
			curr = curr->next;
		}
	}
	return ("");
}

t_env	*create_new_var(char *key, char *content)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (new_node == NULL)
		return (NULL);
	new_node->next = NULL;
	new_node->key = ft_strdup(key);
	if (new_node->key == NULL)
	{
		free(new_node);
		return (NULL);
	}
	new_node->content = ft_strdup(content);
	if (new_node->content == NULL)
	{
		free(new_node->key);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

void	add_var_to_back(t_env **env, t_env *new)
{
	t_env	*curr;

	if (env && new)
	{
		if (*env == NULL)
			*env = new;
		else
		{
			curr = *env;
			while (curr->next)
				curr = curr->next;
			curr->next = new;
		}
	}
}

void	init_env(t_var *data)
{
	extern char	**environ;
	int			i;
	char		**line;
	t_env		*new_var;

	data->env = malloc(sizeof(t_env *));
	if (!data->env)
		safe_exit(data, MALLOC_FAIL);
	*data->env = NULL;
	i = 0;
	while (environ && environ[i])
	{
		line = ft_split(environ[i], '=');
		if (!line)
			safe_exit(data, MALLOC_FAIL);
		if (line[1])
			new_var = create_new_var(line[0], line[1]);
		else
			new_var = create_new_var(line[0], "");
		free_array(line);
		if (!new_var)
			safe_exit(data, MALLOC_FAIL);
		add_var_to_back(data->env, new_var);
		i++;
	}
}

void	free_env(t_env **env)
{
	t_env	*current;
	t_env	*next;

	if (!env)
		return ;
	if (*env)
	{
		current = *env;
		while (current != NULL)
		{
			next = current->next;
			ft_free(&current->content);
			ft_free(&current->key);
			free(current);
			current = next;
		}
	}
	free(env);
	env = NULL;
}
