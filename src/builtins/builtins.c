/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                        :+:       :+:    :+:   */
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
#include "error.h"
#include "utils.h"

/*
Function to call to execute minishell builtin functions + history, true, false.
Returns exit status of executed builtin.
*/
uint8_t	execute_builtin(t_list *tokenlist, t_data *data)
{
	uint8_t	status;

	if (!tokenlist)
		return (EXIT_FAILURE);
	status = 0;
	if (ft_strncmp(get_token_lexeme(tokenlist), "echo", 5) == 0)
		status = execute_echo_builtin(tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "cd", 3) == 0)
		status = execute_cd_builtin(data->env_arr, tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "pwd", 4) == 0)
		status = execute_pwd_builtin();
	else if (ft_strncmp(get_token_lexeme(tokenlist), "export", 7) == 0)
		status = execute_export_builtin(data->env_arr, tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "unset", 6) == 0)
		status = execute_unset_builtin(data->env_arr, tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "env", 4) == 0)
		status = execute_env_builtin(data->env_arr, tokenlist);
	else if (ft_strncmp(get_token_lexeme(tokenlist), "exit", 5) == 0)
		status = (int)execute_exit_builtin(data, tokenlist->next);
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
(from The Open Group Base Specifications Issue 7, 2018 edition:
"When a <hyphen-minus> is used as the operand,
this shall be equivalent to the command: cd "$OLDPWD" && pwd")
*/
uint8_t	execute_cd_builtin(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;
	char	dir[PATH_MAX];
	char	old_dir[PATH_MAX];
	char	*getoldcwd;
	char	*getcwd;

	debug("cd builtin");
	status = 0;
	getoldcwd = NULL;
	getcwd = NULL;
	tokenlist = tokenlist->next;
	if (tokenlist && tokenlist->next)
		return (print_error_status("minishell: cd: too many arguments", 1));
	getoldcwd = execute_getcwd(old_dir, "minishell: cd: get old cwd");
	status = execute_cd_tokenlist(env_arr, tokenlist);
	if (!status)
		return (status);
	getcwd = execute_getcwd(dir, "minishell: cd: get new cwd");
	if (!getoldcwd || !update_env(env_arr, "OLDPWD", old_dir))
		print_error_status("minishell: cd: update of OLDPWD failed", 0);
	if (!getcwd || !update_env(env_arr, "PWD", dir))
		print_error_status("minishell: cd: update of PWD failed", 0);
	return (status);
}

/*
Writes contents of environment to standard output.
No options or arguments implemented.
TODO:
- adding extra arguments after env on the command line changes behavior significantly in BASH
-- e.g. env $HOME, env echo $HOME (mostly 'env' gets ignored in these cases)
*/
uint8_t	execute_env_builtin(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;

	debug("env builtin");
	status = 0;
	if (get_token_lexeme(tokenlist->next))
		status = print_minishell_error_status("env: too many arguments", 1); // include lexeme of argument used
	else
		status = print_env(env_arr);
	return (status);
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
uint8_t	execute_echo_builtin(t_list *tokenlist)
{
	uint8_t	status;
	int		new_line;

	debug("echo builtin");
	status = 0;
	new_line = 1;
	tokenlist = tokenlist->next;
	while (tokenlist && get_token_type(tokenlist) == FLAGS && allowed_flags(get_token_lexeme(tokenlist), "n"))
	{
		new_line = 0;
		tokenlist = tokenlist->next;
	}
	while (tokenlist)
	{
		write_data(1, get_token_lexeme(tokenlist), &status);
		if (tokenlist->next != NULL && token_followed_by_space(tokenlist))
			write_data(1, " ", &status);
		tokenlist = tokenlist->next;
	}
	if (new_line)
		write_data(1, "\n", &status);
	return (status);
}

/*
Executes 'export' builtin. No options interpreted.

- when a string with newline characters is assigned to a variable in BASH
(e.g. VAR="first\nsecond\nthird" - single or double quotes give the same result),
and export, then `export` and `env` list the variable with displayed \n-characters, in env case without quotes, in export case with quotes
but echo on the variable will actually execute the newlines

- If no value is provided export does not assigne a value to `name` and simply displays it as `name` unless the variable already exists, then it is left unchanged
- if export is executed without values but with `export name=` then `export` displays this variable as `name='' (i.e. assigns an empty string)
- while env without arguments does not display the variable at all
- it is possible to assign multiple values to a variable `export VAR=1:2:3:"string" (in this case the quotes are not shown (neither with env nor with export))
- it can also end with a ':'
- export `VAR=2=3` gets added to the environment as `VAR='2=3'` (env would display this variable as `VAR=2=3`)
- export `VAR=` gets added as `VAR=""` (empty string)
- export `VAR==2` gets added as `VAR="=2"
- export VAR="$HOME", VAR=$HOME do not work at the moment (no value assigned at all)
- export VAR="string", VAR="string=string" do not work, the latter assigns a variable with name string and value string
- export var13= "detached string" should return error: bash: export: `detached string': not a valid identifier
- export "var14"=value would assign value to var14
- export "VAR=rew"u=iqorye
QUESTION:
- where do we want to store our local variables?
*/
uint8_t	execute_export_builtin(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;
	char	*key;
	char	*value;
	char	*err_msg;

	debug("export builtin");
	status = 0;
	// print_env(env_arr);
	tokenlist = tokenlist->next;
	if (!tokenlist) // || !get_token_lexeme(tokenlist))
		return (print_env_export(env_arr));
	while (tokenlist)
	{
		key = NULL;
		value = NULL;
		debug("followed by space? %s", (get_curr_token(tokenlist))->folldbyspace ? "true" : "false");
		if (tokenlist->next && !token_followed_by_space(tokenlist))
			status = merge_tokens(tokenlist);
		key = get_var_key(get_token_lexeme(tokenlist));
		value = get_var_value(get_token_lexeme(tokenlist));
		debug("Key: %s, Value: %s", key, value);
		if (no_valid_identifier(key))
		{
			// TODO instead the str3join do maybe a variadic func?
			err_msg = ft_strjoin3("minishell: export `", get_token_lexeme(tokenlist), "': not a valid identifier");
			status = print_error_status(err_msg, 1);
			free(err_msg);
		}
		else if (read_only_variable(key))
		{
			err_msg = ft_strjoin3("minishell: export: ", key, ": readonly variable");
			status = print_error_status(err_msg, 1);
			free(err_msg);
		}
		else if (key && value)
		{
			if (!update_env(env_arr, key, value))
			{
				err_msg = ft_strjoin3("minishell: export: ", value, ": adding to environment failed");
				print_error_status(err_msg, 0);
				free(err_msg);
			}
		}
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
*/
uint8_t	execute_pwd_builtin(void)
{
	char	cur_dir[PATH_MAX];

	debug("pwd builtin");
	if (!getcwd(cur_dir, PATH_MAX))
		return (status_and_perror("minishell: pwd", 1));
	if (printf("%s\n", cur_dir) < 0)
		return (status_and_perror("minishell: pwd: printf", 1));
	return (0);
}

/*
read-only environment variables cannot be unset. How do we manage this?
Do we only work with our local environmental variables or also those of the system?
*/
uint8_t	execute_unset_builtin(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;
	char	*err_msg;
	char	*lexeme;

	status = 0;
	err_msg = NULL;
	debug("unset builtin");
	tokenlist = tokenlist->next;
	if (!tokenlist)
		return (0);
	while (tokenlist)
	{
		lexeme = get_token_lexeme(tokenlist);
		if (ft_strchr(lexeme, '='))
			;
		else if (read_only_variable(lexeme))
		{
			err_msg = ft_strjoin3("minishell: unset: ", lexeme, ": cannot unset: readonly variable");
			status = print_error_status(err_msg, 1);
			free(err_msg);
		}
		else if (lexeme != NULL && ft_strlen(lexeme) != 0)
			status = delete_env_entry(env_arr, lexeme);
		else
			status = print_error_status("minishell: unset: error", 1);
		tokenlist = tokenlist->next;
	}
	return (status);
}

uint8_t	execute_exit_builtin(t_data *data, t_list *tokenlist)
{
	uint8_t	status;
	char	*lexeme;
	char	*message;

	lexeme = get_token_lexeme(tokenlist);
	if (lexeme && ft_isnumstring(lexeme) && tokenlist->next)
		return (print_minishell_error_status("exit: too many arguments", 1));
	restore_fds(data);
	write(1, "exit\n", 5);
	if (lexeme && ft_isnumstring(lexeme))
		status = ft_atoi(lexeme);
	else if (lexeme && !ft_isnumstring(lexeme))
	{
		message = ft_strjoin3("exit: ", lexeme, ": numeric arguments required");
		status = print_minishell_error_status(message, 2);
		free(message);
	}
	else
		status = data->exit_status;
	free_ast(&(data->ast));
	free((char *)(data->input));
	free_data(&data);
	free(data);
	exit(status);
}

bool	ft_isnumstring(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}
