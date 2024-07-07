/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/07 16:26:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executer.h"
#include <limits.h>
#include <libft.h>
#include "utils.h"
#include "splash_error.h"
#include "scanner.h"

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

*/
char	*create_path(char *base, char *path_env)
{
	int		i;
	char	*commandpath;
	char	**envpaths;

	i = 0;
	envpaths = ft_split(path_env, ':');
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
In the case of having a token expanded for ex to "ls -la"
the command would fail because of the space between the flags
I need to retokenize the command so this functions checks for spaces
and creates a new token for each word
*/
void	check_for_spaces(t_list **tokenlist)
{
	char *lex = NULL;

	while (*tokenlist)
	{
		lex = get_token_lexeme(*tokenlist);
		if (ft_strchr(lex, ' '))
		{
			debug("======================== > found space in token: %s", lex);
			t_list *new = tokenizer(lex);
			replace_token_with_tokenlist(tokenlist, new);
		}
		*tokenlist = (*tokenlist)->next;
	}
}


/*
Since until now we store the token as linked list
we convert it to a char array for the execve function
*/ 
char	**get_argv_from_tokenlist(t_list *tokenlist)
{
	int		i;
	char **argv;
	int		count;
	
	i = 0;
	// check_for_spaces(&tokenlist);
	count = count_tokens(tokenlist);
	if (!count)
		return (NULL);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	while (tokenlist)
	{
		argv[i++] = get_token_lexeme(tokenlist);
		tokenlist = tokenlist->next;
	}
	argv[i] = NULL;
	return (argv);
}

/*
resolve_command_path will check if the command is in the PATH
or if it is an absolute path. 
if it cannot be resolved it will return 1
*/
int	resolve_command_path(char **argv, char *path_env)
{
	char	*cmd;

	cmd = NULL;
	if (ft_strchr(argv[0], '/') == NULL)
	{
		cmd = create_path(argv[0], path_env);
		if (!cmd)
			return (print_error_status("minishell: command not on path", 127));
		argv[0] = cmd;
	}
	else
	{
		if (access(argv[0], X_OK) == -1)
			return (status_perror2("minishell: ", argv[0], 126));
	}
	return (0);
}
