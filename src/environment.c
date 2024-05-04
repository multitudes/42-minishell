/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:29:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/29 13:00:00 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"


/*
util function to update the environ variable
This function will join three strings together
*/
char *ft_strjoin3(const char *key, const char *mid, const char *value)
{
	char *new_str1;
	char *new_str2;

	new_str1 = ft_strjoin(key, mid);
	if (!new_str1)
		return (NULL);
	new_str2 = ft_strjoin(new_str1, value);
	if (!new_str2)
	{
		free(new_str1);
		return (NULL);
	}
	free(new_str1);
	return (new_str2);
}

/*
struct s_mini_data 
{
	t_darray	*env_arr;
	t_list		*path_list;
	t_ast_node	*ast;
	char		*heredoc_delimiter;
};
in the environ char ** we have this format: _=/bin/ls
need to test this func!
*/
bool update_env(t_mini_data *data, const char *key, const char *value)
{
	char		*env_str;
	char		*position;
	char		*new_env_str;
	int			i;

	i = 0;
	debug("data->env_arr->end %d\n", data->env_arr->end);
	while (i < data->env_arr->end - 1)
	{
		env_str = darray_get(data->env_arr, i);
		debug("env_str i %d: %s\n", i, env_str);
		position = ft_strchr(env_str, '=');
		debug("position of equal: %s\n", position);
		if (!position)
			return (false);
		if (ft_strncmp(env_str, key, position - env_str) == 0)
		{
			// 	//in the environ char ** we have this format: _=/bin/ls
			debug("found key: %s\n", key);
			new_env_str = ft_strjoin3(key, "=", value);
			debug("new_env_str: %s\n", new_env_str);
			if (!new_env_str)
				return (false);
			darray_set(data->env_arr, i, new_env_str);
			free(env_str);
			return (true);
		}
		i++;
	}
	//if not already in add the new key value pair to the environ
	// new_env_str = ft_strjoin(key, "=");
	// if (!new_env_str)
	// 	return (false);
	// new_env_str = ft_strjoin(new_env_str, value);
	// if (!new_env_str)
	// 	return (false);
	// darray_push(env_arr, new_env_str);
	// darray_push(env_arr, NULL);
	return (true);
}
/*
how to declare a char** str?
char **str;
str = (char **)malloc(sizeof(char *) * 3);
str[0] = "hello";
str[1] = "world";
*/
void print_env(t_mini_data *data)
{
	t_darray	*env_arr;
	int			i;

	i = 0;
	env_arr = data->env_arr;
	printf("printing env\n");
	printf("{");
	while (env_arr->contents[i] != NULL)
		printf("\"%s\",", (char *)darray_get(env_arr, i++));
	printf("}\n");
}

/*
in c and c++ :getenv("_"); will return the value of the last command
for us this function will return the value of the env variable 
given the key..
*/
char *mini_get_env(t_mini_data *data, const char *key)
{
	char		*env_str;
	char		*position;
	int			i;

	i = 0;
	while (i < data->env_arr->end - 1)
	{
		env_str = darray_get(data->env_arr, i);
		position = ft_strchr(env_str, '=');
		if (!position)
			return (NULL);
		if (ft_strncmp(env_str, key, position - env_str) == 0)
			return (position + 1);
		i++;
	}
	return (NULL);
}