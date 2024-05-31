/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:59:50 by rpriess           #+#    #+#             */
/*   Updated: 2024/05/30 16:00:55 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "environment.h"
#include "darray.h"

int	copy_env_darray(t_darray **new_array, t_darray *source_arr)
{
	int		i;
	char	*env;

	debug("copy env darray");
	i = 0;
	*new_array = darray_create(sizeof(char *), source_arr->max);
	if (*new_array == NULL)
		return (1);
	while (source_arr && source_arr->contents && source_arr->contents[i])
	{ 
		env = ft_strdup(source_arr->contents[i++]);
		if (!ft_strncmp(env, "_=", 2))
			continue ;
		if (env == NULL && darray_clear_destroy(*new_array))
			return (1);
		if (darray_push(*new_array, env) == -1 && darray_clear_destroy(*new_array))
			return (1);
	}
	darray_push(*new_array, NULL);
	return (0);
}

int	delete_env_entry(t_darray *env_arr, char *key)
{
	int		i;
	void	*var;

	debug("delete env entry");
	i = get_var_index(env_arr, key);
	if (i < 0)
		return (0);
	else
	{
		var = darray_remove(env_arr, i);
		free(var);
	}
	return (0);
}

/*
return index of an environment (string) variable
with name `key` in an environment array.
If variable does not exist, function returns -1.
*/
int		get_var_index(t_darray *env_arr, const char *key)
{
	int		i;
	char	*var;

	debug("get var index");
	debug("used key: %s and key length: %i", key, (int)ft_strlen(key));
	i = 0;
	while (i < env_arr->end && env_arr->contents[i])
	{
		var = env_arr->contents[i];
		debug("current index and variable: %i %s\n", i, var);
		if (!ft_strncmp(var, key, ft_strlen(key)) && (var[ft_strlen(key)] == '='))
			return (i);
		i++;
	}
	return (-1);
}

//	sort_array_for_export(export_arr);