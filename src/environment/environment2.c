/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:59:50 by rpriess           #+#    #+#             */
/*   Updated: 2024/06/03 11:41:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "darray.h"

int	copy_env_darray(t_darray **new_array, t_darray *source_arr)
{
	int		i;
	char	*env;

	debug("copy env darray");
	i = 0;
	if (!source_arr || !source_arr->contents)
		return (1);
	*new_array = darray_create(sizeof(char *), source_arr->max);
	if (*new_array == NULL)
		return (1);
	while (i < source_arr->end - 1)
	{ 
		env = ft_strdup(source_arr->contents[i++]);
		if (!ft_strncmp(env, "_=", 2) || env == NULL)
			continue ;
		else if (darray_push(*new_array, env) == -1 && darray_clear_destroy(*new_array))
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
	if (ft_strchr(key, '='))
		return (0);
	i = get_var_index(env_arr, key);
	debug("delete %s at index %i", key, i);
	if (i < 0)
		return (0);
	else
	{
		var = darray_remove_and_shift(env_arr, i);
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
	while (i < env_arr->end && i < env_arr->end - 1)
	{
		var = env_arr->contents[i];
		if (!var)
			;
		else if (!ft_strncmp(var, key, ft_strlen(key)) && (var[ft_strlen(key)] == '='))
			return (i);
		i++;
	}
	return (-1);
}

//	sort_array_for_export(export_arr);