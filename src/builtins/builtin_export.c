/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:52:06 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 13:52:10 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"
#include "scanner.h"
#include "parser.h"
#include "environment.h"
#include "builtins.h"
#include "splash_error.h"
#include "debug.h"


static bool	no_valid_identifier(const char *key)
{
	int	i;

	debug("check if valid identifier");
	if (key && (!ft_isalpha(key[0]) || key[0] == '_'))
		return (true);
	debug("First key check passed");
	i = 1;
	while (key[i] != '\0')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (true);
		i++;
	}
	if (key[i] != '\0')
		return (true);
	return (false);
}

static uint8_t process_export_arguments(t_list *tokenlist, t_darray *env_arr)
{
	char	*key;
	char	*value;
	uint8_t	status;

	key = NULL;
	value = NULL;
	status = 0;
	if (tokenlist->next && !token_followed_by_space(tokenlist))
		status = merge_tokens(tokenlist);
	key = get_var_key(get_token_lexeme(tokenlist));
	value = get_var_value(get_token_lexeme(tokenlist));
	debug("Key: %s, Value: %s", key, value);
	if (no_valid_identifier(key))
		status = stderr_and_status3("export `", \
			get_token_lexeme(tokenlist), "': not a valid identifier", 1);
	else if (read_only_variable(key))
		status = stderr_and_status3("export: ", key, \
									": readonly variable", 1);
	else if (key && value)
	{
		if (!update_env(env_arr, key, value))
			stderr_and_status3("export: ", value, \
								": adding to environment failed", 0);
	}
	free(key);
	free(value);
	return (status);
}

/*
Executes 'export' builtin. No options interpreted.

- when a string with newline characters is assigned to a variable in BASH
(e.g. VAR="first\nsecond\nthird" - single or double quotes give the same result),
and export, then `export` and `env` list the variable with displayed \n-characters,
in env case without quotes, in export case with quotes and escape charaters
but echo on the variable will actually execute the newlines

- If no value is provided export does not assigne a value to `name` 
	and simply displays it as `name`
	unless the variable already exists, then it is left unchanged
- if export is executed without values but with `export name=` 
	then `export` displays this variable as `name=''
	(i.e. assigns an empty string)
- while env without arguments does not display the variable at all
- it is possible to assign multiple values to a variable 
	`export VAR=1:2:3:"string" (in this case the quotes are not shown 
	(neither with env nor with export))
- it can also end with a ':'
- export `VAR=2=3` gets added to the environment as `VAR='2=3'` 
	(env would display this variable as `VAR=2=3`)
- export `VAR=` gets added as `VAR=""` (empty string)
- export `VAR==2` gets added as `VAR="=2"
- export VAR="$HOME", VAR=$HOME do not work at the moment
	(no value assigned at all)
- export VAR="string", VAR="string=string" do not work,
	the latter assigns a variable with name string and value string
- export var13= "detached string" should return 
	error: bash: export: `detached string': not a valid identifier
- export "var14"=value would assign value to var14
- export "VAR=rew"u=iqorye
QUESTION:
- where do we want to store our local variables?
*/
uint8_t	execute_export_builtin(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;

	debug("export builtin");
	status = 0;
	tokenlist = tokenlist->next;
	if (!tokenlist)
		return (print_env_export(env_arr));
	while (tokenlist)
	{
		debug("followed by space? %s", (get_curr_token(tokenlist))->folldbyspace ? "true" : "false");
		status = process_export_arguments(tokenlist, env_arr);
		tokenlist = tokenlist->next;
	}
	return (status);
}
