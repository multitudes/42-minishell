/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 11:40:17 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/14 12:02:51 by lbrusa           ###   ########.fr       */
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
		return (stderr_and_status("malloc data", 0));
	(*data)->env_arr = env_array;
	shlvl_init(*data);
	return (init_data2(data));
}

bool	init_data2(t_data **data)
{
	(*data)->input = "no input";
	(*data)->homepath = NULL;
	(*data)->tokenlist = NULL;
	(*data)->ast = NULL;
	(*data)->exit_status = 0;
	(*data)->original_stdout = -1;
	(*data)->original_stdin = -1;
	(*data)->original_stderr = -1;
	(*data)->pipe_fd[0] = -1;
	(*data)->pipe_fd[1] = -1;
	(*data)->pipe_open = false;
	return (true);
}

bool	init_env_darray(t_darray **env_arr)
{
	int		i;
	char	*env;

	i = 0;
	*env_arr = darray_create(sizeof(char *), 100);
	if (*env_arr == NULL)
		return (stderr_and_status("error setup env", 0));
	while (environ && environ[i])
	{
		env = ft_strdup(environ[i++]);
		if (env == NULL && darray_clear_destroy(*env_arr))
			return (stderr_and_status("malloc env", 0));
		if (darray_push(*env_arr, env) == -1 && darray_clear_destroy(*env_arr))
			return (stderr_and_status("error setup env", 0));
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
