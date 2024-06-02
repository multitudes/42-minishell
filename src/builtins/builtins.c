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
	int		status;

	if (!tokenlist)
		return (EXIT_FAILURE);
	status = 0;
	if (ft_strncmp(get_token_lexeme(tokenlist), "echo", 5) == 0)
		status = execute_echo_builtin(tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "cd", 3) == 0)
		status = execute_cd_builtin(data, tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "pwd", 4) == 0)
		status = execute_pwd_builtin();
	else if (ft_strncmp(get_token_lexeme(tokenlist), "export", 7) == 0)
		status = execute_export_builtin(data, tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "unset", 6) == 0)
		status = execute_unset_builtin(data, tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "env", 4) == 0)
		status = execute_env_builtin(data);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "exit", 5) == 0)
	{
		debug("exit builtin");
	}
	else if (ft_strncmp(get_token_lexeme(tokenlist), "true", 5) == 0)
	{
		status = 0;
		debug("true builtin");
	}
	else if (ft_strncmp(get_token_lexeme(tokenlist), "false", 6) == 0)
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
-- if invalid name is given output is "bash: cd: ARGUMENT_GIVEN: No such file or directory" with exit code 1
-- If directory is '-', it is converted to $OLDPWD before attempting directory change.
(from The Open Group Base Specifications Issue 7, 2018 edition:
"When a <hyphen-minus> is used as the operand,
this shall be equivalent to the command: cd "$OLDPWD" && pwd")
-- when $HOME is undefined just performing `cd` is implementation defined (How is this handled in BASH?)
*/
int	execute_cd_builtin(t_data *data, t_list *tokenlist)
{
	int		cd_return;
	int		status;
	char	*dir;

	debug("cd builtin");
	tokenlist = tokenlist->next;
	dir = getcwd(NULL, 0);
	if (!update_env(data->env_arr, "OLDPWD", dir))
		status = 1;
	free(dir);
	if (tokenlist->next)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (tokenlist)
	{
		cd_return = chdir(get_token_lexeme(tokenlist));
		if (cd_return != 0)
			status = 1; // 1 is the exit status but prints minishell: cd (dir) : No such file or directory
		dir = getcwd(NULL, 0);
		if (!update_env(data->env_arr, "PWD", dir))
			status = 1;
		free(dir);
	}
	else
	{
		cd_return = chdir(mini_get_env(data->env_arr, "HOME"));
		if (cd_return != 0)
			status = 1;
		dir = getcwd(NULL, 0);
		if (!update_env(data->env_arr, "PWD", dir))
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

bool	write_data(int fd, const void *str, int *status) 
{
    ssize_t result; 
	
	result = write(fd, str, ft_strlen(str));
    if (result == -1 || result != (ssize_t)ft_strlen(str)) 
	{
        perror("write");
		*status = EXIT_FAILURE;
		return (false);
    }
	return (true);
}

bool	allowed_flags(const char *flag_lexem, const char *allowed_flags)
{
	flag_lexem++;
	while (*flag_lexem)
	{
		if (!ft_strchr(allowed_flags, *flag_lexem))
			return (false);
		flag_lexem++;
	}
	return (true);
}

/*
Executes builtin "echo" function with and without '-n' option
which suppresses the trailing newline.
*/
int	execute_echo_builtin(t_list *tokenlist)
{
	int		status;
	int		new_line;

	debug("echo builtin");
	status = 0;
	new_line = 1;
	tokenlist = tokenlist->next;
	while (tokenlist)
	{
		debug("lexeme: %s", get_token_lexeme(tokenlist));
		if (get_token_type(tokenlist) == FLAGS && allowed_flags(get_token_lexeme(tokenlist), "n"))
				new_line = 0;
		else
		{
			write_data(1, get_token_lexeme(tokenlist), &status);
			if (tokenlist->next != NULL)
				write_data(1, " ", &status);
		}
		tokenlist = tokenlist->next;
	}
	if (new_line)
		write_data(1, "\n", &status);		
	return (status);
}

/*
Executes 'export' builtin. No options interpreted.
- No arguments should give a sorted list of the environment variables (lower case variables come after all upper case variables) with `declare -x ` prepended,
variable values are in double quotes and apparently the last command (i.e. `export`) is not included
- when a string with newline characters is assigned to a variable in BASH
(e.g. VAR="first\nsecond\nthird" - single or double quotes give the same result),
and export, then `export` and `env` list the variable with displayed \n-characters, in env case without quotes, in export case with quotes
but echo on the variable will actually execute the newlines
- it should be possible to export several variables at once, while also giving a value or not.
- If no value is provided export does not assigne a value to `name` and simply displays it as `name` unless the variable already exists, then it is left unchanged
- if export is executed without values but with `export name=` then `export` displays this variable as `name='' (i.e. assigns an empty string)
- while env without arguments does not display the variable at all
- it is possible to assign multiple values to a variable `export VAR=1:2:3:"string" (in this case the quotes are not shown (neither with env nor with export))
- it can also end with a ':'
- export `VAR=2=3` gets added to the environment as `VAR='2=3'` (env would display this variable as `VAR=2=3`)
- export `VAR=` gets added as `VAR=""` (empty string)
- export `VAR==2` gets added as `VAR="=2"`
QUESTION:
- where do we want to store our local variables?
*/
int	execute_export_builtin(t_data *data, t_list *tokenlist)
{
	int		status;
	char	*key;
	char	*value;

	debug("export builtin");
	status = 0;
	tokenlist = tokenlist->next;
	if (!tokenlist)
		status = print_env_export(data->env_arr);
	while (tokenlist)
	{
		// if VAR without '=' but exists as local variable, add that variable to env array
		// if VAR without '=' add variable (needs to be interpreted differently in buitlin "env", i.e. not being shown at all)
		key = get_var_key(get_token_lexeme(tokenlist));
		value = get_var_value(get_token_lexeme(tokenlist));
		debug("Key: %s, Value: %s", key, value);
		if (key && value != NULL)
			update_env(data->env_arr, key, value);
		free(key);
		free(value);
		// if VAR wigh '=' and nothing else, update VAR if it already exists, else add as VAR=
		// else update value if key exists
		//else add argument as new entry in env arr
		tokenlist = tokenlist->next;
	}
	return (status);
}

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
		debug("Error reading current directory with getcwd()");
	}
	status = printf("%s\n", current_directory);
	free(current_directory);
	if (status < 0)
		return (1);
	return (0);
}

/*
read-only environment variables cannot be unset. How do we manage this?
Do we only work with our local environmental variables or also those of the system?
*/
int	execute_unset_builtin(t_data *data, t_list *tokenlist)
{
	t_token *token;

	debug("unset builtin");
	tokenlist = tokenlist->next;
	if (!tokenlist)
		return (0);
	while (tokenlist)
	{
		token = tokenlist->content;
		if (token->lexeme != NULL && ft_strlen(token->lexeme) != 0)
			delete_env_entry(data->env_arr, token->lexeme);
	// if (restricted_variable(token->lexeme))
	// {
	// 	//consider returning error message in restricted_variable() function, also consider other possible restricted variables.
	// 	write(2, "minishell: unset: ", 18);
	// 	write(2, &token->lexeme, ft_strlen(token->lexeme));
	// 	write(2, ":cannot unset: readonly variable", 32);
	// 	return (1);
	// }
		tokenlist = tokenlist->next;
	}
	return (0);
}
