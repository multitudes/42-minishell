/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 11:40:17 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/12 11:52:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include "init.h"
#include "minishell.h"
#include "splash_error.h"

/*
The environ variable is part of the POSIX standard, so it should be 
available on any POSIX-compliant system.
according to the linux programming language by kerrisk (page 127), using
the environ variable is better than getting it in main.. (not posix compliant)
*/
extern char	**environ;

/*
initialiser for data
It would be nice to to save the home dir at the very beginning
*/
bool	init_data(t_data **data)
{
	t_darray	*env_array;

	if (!init_env_darray(&env_array))
		return (false);
	*data = malloc(sizeof(t_data));
	if (*data == NULL && darray_clear_destroy(env_array))
		return (zero_and_printerr("malloc data"));
	(*data)->env_arr = env_array;
	return (init_data2(data));
}

bool	init_data2(t_data **data)
{
	(*data)->input = "no input";
	(*data)->tokenlist = NULL;
	(*data)->ast = NULL;
	(*data)->exit_status = 0;
	shlvl_init(*data);
	return (true);
}

bool	init_env_darray(t_darray **env_arr)
{
	int		i;
	char	*env;

	i = 0;
	*env_arr = darray_create(sizeof(char *), 100);
	if (*env_arr == NULL)
		return (zero_and_printerr("env_array env"));
	while (environ && environ[i])
	{
		env = ft_strdup(environ[i++]);
		if (env == NULL && darray_clear_destroy(*env_arr))
			return (zero_and_printerr("malloc env"));
		if (darray_push(*env_arr, env) == -1 && darray_clear_destroy(*env_arr))
			return (zero_and_printerr("darray_push"));
	}
	darray_push(*env_arr, NULL);
	return (true);
}

/*
with each new instance of the shell,
the env variable SHLVL is increased by one.
*/
void	shlvl_init(t_data *data)
{
	char	*shlvl;

	shlvl = ft_itoa(ft_atoi(mini_get_env(data->env_arr, "SHLVL")) + 1);
	update_env(data->env_arr, "SHLVL", shlvl);
	free(shlvl);
}