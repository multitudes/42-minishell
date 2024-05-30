/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 18:54:33 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

/*
when I need to free a string array like the envpaths
*/
int	free_array(char **envpaths)
{
	int	i;

	i = 0;
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
char	*create_path(char *base, t_data *data)
{
	int		i;
	char	*commandpath;
	char	**envpaths;

	i = 0;
	envpaths = ft_split(mini_get_env(data->env_arr, "PATH"), ':');
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

int	count_tokens(t_list *tokenlist)
{
	int	count;

	count = 0;
	while (tokenlist) 
	{
		count++;
		tokenlist = tokenlist->next;
	}
	return (count);
}

/*
Since until now we store the token as linked list
we convert it to a char array for the execve function
*/ 
char	**get_args_from_tokenlist(t_list *tokenlist)
{
	int		i;
	int		count;
	char	**args;
	t_token	*token;

	i = 0;
	token = (t_token *)tokenlist->content;
	count = count_tokens(tokenlist);
	if (!count)
		return (NULL);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	while (tokenlist)
	{
		token = (t_token *)tokenlist->content;
		args[i++] = token->lexeme;
		tokenlist = tokenlist->next;
	}
	args[i] = NULL;
	return (args);
}

/*
resolve_command_path will check if the command is in the PATH
or if it is an absolute path. 
if it cannot be resolved it will return 1
*/
int	resolve_command_path(char **argv, t_data *data)
{
	char	*cmd;

	cmd = NULL;
	if (ft_strchr(argv[0], '/') == NULL)
	{
		cmd = create_path(argv[0], data);
		if (!cmd)
			return (status_and_print("minishell: command not on path\n", 1));
		argv[0] = cmd;
	}
	else
	{
		if (access(argv[0], X_OK) == -1)
			return (status_and_print("minishell: command not found\n", 1));
	}
	return (0);
}
