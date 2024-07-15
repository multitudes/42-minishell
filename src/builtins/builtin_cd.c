/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:52:22 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 13:52:25 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"
#include "scanner.h"
#include "parser.h"
#include "splash_error.h"
#include "environment.h"
#include "analyser.h"
#include "debug.h"
#include "builtins.h"

static char *execute_getcwd(char old_dir[]) // char *message)
{
    char    *retval;

    retval = NULL;
    retval = getcwd(old_dir, PATH_MAX);
    return (retval);
}

static uint8_t execute_cd_tokenlist(t_darray *env_arr, t_list *tokenlist)
{
	char *home;
    char *lexeme;

	debug("execute_cd_tokenlist with lexeme %s", get_token_lexeme(tokenlist));
	home = mini_get_env(env_arr, "HOME");
	lexeme = get_token_lexeme(tokenlist);
    if (tokenlist && lexeme) 
	{
		if (ft_strncmp(get_token_lexeme(tokenlist), "-", 2) == 0) // one arg  '-'
		{
			if (!mini_get_env(env_arr, "OLDPWD"))
				return (stderr_and_status("cd: OLDPWD not set", 1));
			else
			{
				if (chdir(mini_get_env(env_arr, "OLDPWD")))
					return (perror_and_status("cd", 1));
				execute_pwd_builtin();
			}
		}
		else if (*lexeme && chdir(get_token_lexeme(tokenlist)))
            return (perror_and_status2("cd: ", get_token_lexeme(tokenlist), 1));
    }
    else // no args
	{
        if (!home)
			return (stderr_and_status("cd: HOME not set", 1));
		home = get_home(env_arr);
        debug("home is -%s--------------------", home);
		if (home && *home != '\0' && chdir(home))
		{
			free(home);
			return (perror_and_status("cd", 1));
		}
		free(home);
	}
    return (0);
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
		return (stderr_and_status("cd: too many arguments", 1));
	getoldcwd = execute_getcwd(old_dir);
	status = execute_cd_tokenlist(env_arr, tokenlist);
	debug("status: %d", status);
	if (status != 0)
		return (status);
	getcwd = execute_getcwd(dir);
	if (!getoldcwd || !getcwd)
		status = perror_and_status("getcwd", 0);
	else if (!update_env(env_arr, "PWD", dir) \
			|| !update_env(env_arr, "OLDPWD", old_dir))
		status = stderr_and_status("cd: env update failed", 0);
	return (status);
}

