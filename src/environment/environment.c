/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:29:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/06 15:52:03 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"
#include "darray.h"
#include "utils.h"
#include "splash_error.h"

/*
Update / add a variable to the environment given a key and corresponding value.
*/

bool update_env(t_darray *env_arr, const char *key, const char *value)
{
	char		*env_str;
	char		*new_env_str;
	int			i;

	new_env_str = ft_strjoin3(key, "=", value);
	if (!new_env_str)
		return (false);
	i = get_var_index(env_arr, key);
	if (i < 0)
	{
		darray_set(env_arr, env_arr->end - 1, new_env_str);
		darray_push(env_arr, NULL);
	}
	else
	{
		env_str = darray_get(env_arr, i);
		free(env_str);
		darray_set(env_arr, i, new_env_str);
	}
	return (true);
}

/*
Function prints the environment variables
stored in environment array
*/
int	print_env(t_darray *env_arr)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (env_arr && env_arr->contents && i < env_arr->end - 1)
	{
		if (env_arr->contents[i] == NULL)
			;
		else if (printf("%s\n", (char *)darray_get(env_arr, i)) < 0)
			status = perror_and_status("printf environment", 1);
		i++;
	}
	return (status);
}

/*
Returns the value of an environment variable given a key.
The returned value is not allocated.
*/
char *mini_get_env(t_darray *env_arr, const char *key)
{
	char		*env_str;
	char		*position;
	int			i;

	if (!env_arr || !key)
		return (NULL);
	if (ft_strchr(key, '='))
		return (NULL);
	i = 0;
	while (key && i < env_arr->end - 1)
	{
		env_str = darray_get(env_arr, i);
		position = ft_strchr(env_str, '=');
		if (!position)
			return (NULL);
		if (ft_strncmp(env_str, key, position - env_str) == 0 \
				&& (position - env_str) == (int)ft_strlen(key))
			return (position + 1);
		i++;
	}
	return (NULL);
}

/*
Prints the environment variables when `export` command is used:
Quoted strings, last command is omitted.
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
	while (export_arr && export_arr->contents && i < env_arr->end - 1)
	{
		key = get_var_key(export_arr->contents[i++]);
		value = mini_get_env(export_arr, key);
		if (key \
			&& (printf("declare -x %s=%c%s%c\n", key, '"', value, '"') < 0))
			status = perror_and_status("printf export", 1);
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
	if (position)
		key = ft_substr(variable, 0, position - variable);
	else
		key = ft_strdup(variable);
	return (key);
}

/*
Returns the value in a variable string.
Memory for value is malloced and needs to be freed.
*/
char	*get_var_value(const char *variable)
{
	char	*position;
	char	*value;
	position = ft_strchr(variable, '=');
	if (position)
		value = ft_substr(variable, position - variable + 1, \
				ft_strlen(position + 1));
	else
		value = NULL;
	return (value);
}