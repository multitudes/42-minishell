/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 22:01:13 by rpriess           #+#    #+#             */
/*   Updated: 2024/05/28 08:26:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "libft.h"
#include "scanner.h"
#include "environment.h"
#include "history.h"
#include <unistd.h>

/*
Function to call to execute minishell builtin functions + history, true, false.
Returns exit status of executed builtin.
*/
int    execute_builtin(t_list *tokenlist, t_data *data)
{
	t_token *token;
	char 	*lexeme;
	int		status;
//	(void)data; //I think this does not do anything!?

	if (tokenlist)
		token = (t_token *)tokenlist->content;
	else
		return (1);
	lexeme = (char *)token->lexeme;
	status = 0;
	if (ft_strncmp(lexeme, "echo", 5) == 0)
		status = execute_echo_builtin(tokenlist);
	else if (ft_strncmp(lexeme, "cd", 3) == 0)
		status = execute_cd_builtin(data, tokenlist);
	else if (ft_strncmp(lexeme, "pwd", 4) == 0)
		status = execute_pwd_builtin();
	else if (ft_strncmp(lexeme, "export", 7) == 0)
		debug("export builtin");
//		status = execute_export_builtin(data, tokenlist);
	else if (ft_strncmp(lexeme, "unset", 6) == 0)
	{
		debug("unset builtin");
	}
	else if (ft_strncmp(lexeme, "env", 4) == 0)
		status = execute_env_builtin(data);
	else if (ft_strncmp(lexeme, "exit", 5) == 0)
	{
		debug("exit builtin");
	}
	else if (ft_strncmp(lexeme, "true", 5) == 0)
	{
		status = 0;
		debug("true builtin");
	}
	else if (ft_strncmp(lexeme, "false", 6) == 0)
	{
		status = 1;
		debug("false builtin");
	}
	else if (ft_strncmp(data->input, "history -c", 11) == 0 || ft_strncmp(data->input, "history --clear", 16) == 0)
	{
		// debug("clearing history\n");
		clear_hist_file();
		rl_clear_history();
	}
	else if (ft_strncmp(data->input, "history", 7) == 0)
		print_history(); //we need exit status of history command ! (after all) ;)
	else
	{
		debug("not an implemented builtin");
	}
	return (status);
}

/*
Minishell builtin for "cd" command.
Relative and absolute path as arguments.
Updates PWD and OLDPWD environment variables.
TODOs:
- interpret errors from called getcwd system function.
- Implement additional functionality:
-- when additional invaldid arguments are given the command should abort and an error message given
-- when command 'cd $HOME $HOME' is entered in BASH the current directory is output in relative format (which is a strange behavior)
-- If directory is '-', it is converted to $OLDPWD before attempting directory change.
*/
int	execute_cd_builtin(t_data *data, t_list *tokenlist)
{
	t_token	*token;
	int		cd_return;
	int		status;
	char	*dir;

	debug("cd builtin");
	token = tokenlist->content;
	if (ft_strncmp(token->lexeme, "cd", 3) == 0)
	{
		debug("lexeme: %s (Not printed)", token->lexeme);
		tokenlist = tokenlist->next;
	}
	dir = getcwd(NULL, 0);
	if (!update_env(data, "OLDPWD", dir))
		status = 1;
	free(dir);
	if (tokenlist)
	{
		token = tokenlist->content;
		cd_return = chdir(token->lexeme);
		if (cd_return != 0)
			status = 1;
		dir = getcwd(NULL, 0);
		if (!update_env(data, "PWD", dir))
			status = 1;
		free(dir);
	}
	else
	{
		cd_return = chdir(mini_get_env(data, "HOME"));
		if (cd_return != 0)
			status = 1;
		dir = getcwd(NULL, 0);
		if (!update_env(data, "PWD", dir))
			status = 1;
		free(dir);
	}		
	return (status);
}

/*
Writes contents of environment to standard output.
No options or arguments implemented.
TODO:
- adding extra arguments after env on the command line changes behavior significantly in BASH
-- e.g. env $HOME, env echo $HOME (mostly 'env' gets ignored in these cases)
*/
int	execute_env_builtin(t_data *data)
{
	int	status;

	debug("env builtin");
	status = 0;
	status = print_env(data->env_arr);
	return (status);
}

/*
Executes builtin "echo" function with and without '-n' option.
TO CHECK:
When suppressing the newline at the end, does echo actually introduse a carriage return or similar?
*/
int	execute_echo_builtin(t_list *tokenlist)
{
	int		status;
	t_token	*token;
	int		new_line;
	int		printf_return;

	debug("echo builtin");
	status = 0;
	new_line = 1;
	printf_return = 0;
	token = tokenlist->content;
	if (ft_strncmp(token->lexeme, "echo", 5) == 0)
	{
		debug("lexeme: %s (Not printed)", token->lexeme);
		tokenlist = tokenlist->next;
	}
	else
	{
		debug("echo builtin falsely called");
		return (1);
	}
	if (tokenlist && ft_strncmp(((t_token *)tokenlist->content)->lexeme, "-n", 3) == 0)
	{
		token = tokenlist->content;
		new_line = 0;
		debug("lexeme: %s (newline at end to be suppressed)", token->lexeme);
		tokenlist = tokenlist->next;
	}
	while (tokenlist)
	{
		token = tokenlist->content;
		debug("lexeme: %s", token->lexeme);
		if (tokenlist->next)
			printf_return = printf("%s ", token->lexeme);
		else
			printf_return = printf("%s", token->lexeme);
		tokenlist = tokenlist->next;
		if (printf_return < 0)
			status = 1;
	}
	if (new_line == 1)
			printf_return = printf("\n");
	return (status);
}

/*
Executes 'export' builtin. No options interpreted.
- No arguments should give a sorted list of the environment variables (lower case variables come after all upper case variables),
a few variables are quoted and apparently the last command is not included
- when a string with newline characters is assigned to a variable in BASH
(e.g. VAR="first\nsecond\nthird" - single or double quotes give the same result),
and export, then `export` and `env` list the variable with displayed \n-characters, in env case without quotes, in export case with quotes
but echo on the variable will actually execute the newlines
- it should be possible to export several variables at once, while also giving a value or not.
- If no value is provided export assigns empty string to `name` unless the variable already exists.
- export without arguments then display this variable as `name=''
- while env without arguments does not display the variable.
- it is possible to assign multiple value to a variable `export VAR=1:2:3:"string" (in this case the quotes are not shown (neither with env nor with export))
- it can also end with a ':'
QUESTION:
- where do we want to store our local variables?
*/
/*
int	execute_export_builtin(t_data *data, t_list *tokenlist)
{
	t_token	*token;

	debug("export builtin");
	token = tokenlist->content;
	if (ft_strncmp(token->lexeme, "export", 7) == 0)
	{
		debug("lexeme: %s (Not printed)", token->lexeme);
		tokenlist = tokenlist->next;
	}
	if (!tokenlist)
		debug("Print sorted and reformatted env list here.");
	if (tokenlist)
		debug("Parse arguments for export builtin.");
}*/

/*
Executes builtin 'pwd' command.
TODO:
- interpret possible errors from system function `getcwd()`
*/
int	execute_pwd_builtin(void)
{
	int		status;
	char	*current_directory;

	debug("pwd builtin");
	current_directory = getcwd(NULL, 0);
	if (!current_directory)
	{
		debug("Buffer for reading current directory too small");
	}
	status = printf("%s\n", current_directory);
	free(current_directory);
	if (status < 0)
		return (1);
	return (0);
}

