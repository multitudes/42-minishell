/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:29:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/18 15:51:50 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"
#include "darray.h"

bool update_env(t_darray *env_arr, const char *key, const char *value)
{
	char		*env_str;
	char		*position;
	char		*new_env_str;
	int			i;

	i = 0;
	// debug("data->env_arr->end %d\n", data->env_arr->end);
	while (i < env_arr->end - 1)
	{
		env_str = darray_get(env_arr, i);
		// debug("env_str i %d: %s\n", i, env_str);
		position = ft_strchr(env_str, '=');
		// debug("position of equal: %s\n", position);
		if (!position)
			return (false);
		if (ft_strncmp(env_str, key, position - env_str) == 0)
		{
			// 	//in the environ char ** we have this format: _=/bin/ls
			// debug("found key: %s\n", key);
			new_env_str = ft_strjoin3(key, "=", value);
			// debug("new_env_str: %s\n", new_env_str);
			if (!new_env_str)
				return (false);
			darray_set(env_arr, i, new_env_str);
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
this function will print the environment variables
*/
int	print_env(t_darray *env_arr)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (env_arr && env_arr->contents && env_arr->contents[i] != NULL)
	{
		if (printf("%s\n", (char *)darray_get(env_arr, i++)) < 0)
			status = 1;
	}
	return (status);
}

/*
in c and c++ :getenv("_"); will return the value of the last command
for us this function will return the value of the env variable 
given the key..
*/
char *mini_get_env(t_darray *env_arr, const char *key)
{
	char		*env_str;
	char		*position;
	int			i;

	i = 0;
	while (i < env_arr->end - 1)
	{
		env_str = darray_get(env_arr, i);
		position = ft_strchr(env_str, '=');
		if (!position)
			return (NULL);
		if (ft_strncmp(env_str, key, position - env_str) == 0)
			return (position + 1);
		i++;
	}
	return (NULL);
}

/*
print the environment variables when `export` command is used:
Sorted, quoted strings, last command omitted
*/
int	print_env_export(t_darray *env_arr)
{
	t_darray	*export_arr;
	int			status;
	char		*key;
	char		*value;
	int			i;

	debug("print env with `export`");
	status = 0;
	i = 0;
	export_arr = NULL;
	if (copy_env_darray(&export_arr, env_arr))
		return (1);
//	sort_array_for_export(export_arr);
	print_env(export_arr);
	while (export_arr && export_arr->contents && export_arr->contents[i] != NULL)
	{
		key = get_var_key(export_arr->contents[i++]);
		value = mini_get_env(export_arr, key);
		if (printf("declare -x %s=%c%s%c\n", key, '"', value, '"') < 0)
			status = 1;
		free(key);
	}
	darray_clear_destroy(export_arr);
	return (status);
}

/*
Returns the key in a variable string.
Memory for key is malloced and needs to be freed.
*/
char	*get_var_key(const char *variable)
{
	char	*position;
	char	*key;
	position = ft_strchr(variable, '=');
	key = ft_substr(variable, 0, position - variable);
	return (key);
}