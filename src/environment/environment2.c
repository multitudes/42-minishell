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

/*
int	add_quotes_env_values(t_darray *env_arr)
{
	int		i;
	int		position;
	char	*new_env_str;
	char	*substr_1;
	char	*substr_2;
	char	*new_value;
	char	*key;

	i = 0;
	if (env_arr == NULL)
		return (1);
	while (env_arr[i])
	{
		temp = ;
		position = ft_strchr(darray_get(env_arr, i), '=');
		key = ft_substr(darray_get(env_arr, i), 0, position - env_arr[i])
		update_env(env_arr, key)

		new_env_str = ft_strjoin3("declare -x ", "substr_1", "substr_2");
	}
}*/

//	sort_array_for_export(export_arr);