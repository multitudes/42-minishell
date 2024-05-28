/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 08:31:26 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

/*
when I need to free a string array like the envpaths
*/
int free_array(char **envpaths)
{
	int i = 0;
	while (envpaths[i])
	{
		free(envpaths[i]);
		i++;
	}
	free(envpaths);
	return (0);
}

/*
In our data struct we have the environment variables in a dynamic array
mini_get_env will get the path variable and return it
as a string array.
base is the command we are int count_tokens(t_list *tokenlist) 
{
	int count;

	count = 0;
	while (tokenlist) {
		count++;
		tokenlist = tokenlist->next;
	}
	return (count);
}looking for.
*/
char *create_path(char *base, t_data *data)
{
	int i;
	char *commandpath;
	char **envpaths;

	i = 0;
	envpaths = ft_split(mini_get_env(data, "PATH"), ':');
	while (envpaths[i])
	{
		commandpath = ft_strjoin3(envpaths[i], "/", base);
		if (access(commandpath, X_OK) == 0)
		{
			free_array(envpaths);
			return (commandpath);
		}
		else
			free(commandpath);
		i++;
	}
	free_array(envpaths);
	return (NULL);
}

int count_tokens(t_list *tokenlist) 
{
	int count;

	count = 0;
	while (tokenlist) {
		count++;
		tokenlist = tokenlist->next;
	}
	return (count);
}
