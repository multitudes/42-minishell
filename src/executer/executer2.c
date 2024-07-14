/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 20:34:06 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executer.h"
#include <limits.h>
#include <libft.h>
#include "utils.h"
#include "splash_error.h"
#include "scanner.h"
#include <sys/stat.h>

/*
resolve_command_path will check if the command is in the PATH
or if it is an absolute path. 
if it cannot be resolved it will return 1
*/
int	resolve_command_path(char **argv, char *path_env)
{
	struct stat	statbuf;
	int			is_relative_path;

	if (!find_path(argv, path_env))
		return (print_error_status2(argv[0], ": command not found", 127));
	if (access(argv[0], F_OK) == -1)
		return (status_perror2("minishell: ", argv[0], 127));
	if (stat(argv[0], &statbuf))
		return (status_perror2("minishell: ", argv[0], 1));
	if (S_ISDIR(statbuf.st_mode))
		return (print_error_status2(argv[0], ": is a directory", 126));
	is_relative_path = ft_strncmp(argv[0], "./", 2) == 0 || \
						ft_strcmp(argv[0], "..") != 0;
	if (is_relative_path && access(argv[0], X_OK) == -1)
		return (status_perror2("minishell: ", argv[0], 126));
	return (0);
}

/*
 * used by resolve_command_path when I get a simple command like "ls"
 * I need to check if the command is in the PATH
*/
bool	find_path(char **argv, char *path_env)
{
	char		*cmd;

	cmd = NULL;
	if (ft_strchr(argv[0], '/') == NULL)
	{
		cmd = create_path(argv[0], path_env);
		if (!cmd || ft_strcmp(argv[0], "..") == 0)
		{
			free(cmd);
			return (false);
		}
		argv[0] = cmd;
	}
	return (true);
}

/*
 * used by find_path to create the path to the command
 * 
*/
char	*create_path(char *base, char *path_env)
{
	int		i;
	char	**envpaths;
	char	*commandpath;

	i = 0;
	commandpath = NULL;
	if (!path_env || !base)
		return (NULL);
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
In the case of having a token expanded for ex to "ls -la"
the command would fail because of the space between the flags
I need to retokenize the command so this functions checks for spaces
and creates a new token for each word
*/
void	check_for_spaces(t_list **tokenlist)
{
	char	*lex;
	t_list	*new;
	t_list	*head;

	lex = NULL;
	new = NULL;
	head = *tokenlist;
	while (*tokenlist)
	{
		lex = get_token_lexeme(*tokenlist);
		if (ft_strchr(lex, ' ') && get_token_type(*tokenlist) == EXPANDED)
		{
			new = tokenizer(lex);
			replace_token_with_tokenlist(&head, tokenlist, new);
		}
		if (!(*tokenlist)->next)
			break ;
		*tokenlist = (*tokenlist)->next;
	}
	head = get_head(*tokenlist);
	*tokenlist = head;
}

/*
Since until now we store the token as linked list
we convert it to a char array for the execve function
*/ 
char	**get_argv_from_tokenlist(t_list **tokenlist)
{
	int		i;
	int		count;
	char	**argv;

	i = 0;
	check_for_spaces(tokenlist);
	count = count_tokens(*tokenlist);
	if (!count)
		return (NULL);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	while (*tokenlist)
	{
		argv[i++] = get_token_lexeme(*tokenlist);
		*tokenlist = (*tokenlist)->next;
	}
	argv[i] = NULL;
	return (argv);
}
