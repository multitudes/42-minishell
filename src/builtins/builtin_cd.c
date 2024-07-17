/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:52:22 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/16 15:18:59 by lbrusa           ###   ########.fr       */
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

static uint8_t	exec_cd_without_argument(t_darray *env_arr, t_list *tokenlist)
{
	char	*home;

	home = mini_get_env(env_arr, "HOME");
	if (!home)
		return (stderr_and_status("cd: HOME not set", 1));
	if (home && *home != '\0' && chdir(home))
		return (perror_and_status2("cd: ", get_token_lexeme(tokenlist), 1));
	return (0);
}

static uint8_t	exec_cd_with_argument(t_darray *env_arr, t_list *tokenlist)
{
	if (ft_strcmp(get_token_lexeme(tokenlist), "-") == 0)
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
	else if ((get_token_lexeme(tokenlist))[0] \
				&& chdir(get_token_lexeme(tokenlist)))
		return (perror_and_status2("cd: ", get_token_lexeme(tokenlist), 1));
	return (0);
}

static char	*execute_getcwd(char old_dir[])
{
	char	*retval;

	retval = NULL;
	retval = getcwd(old_dir, PATH_MAX);
	return (retval);
}

static uint8_t	execute_cd_tokenlist(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;

	status = 0;
	if (tokenlist && get_token_lexeme(tokenlist))
		status = exec_cd_with_argument(env_arr, tokenlist);
	else
		status = exec_cd_without_argument(env_arr, tokenlist);
	return (status);
}

/*
Minishell builtin for "cd" command. Also cd ~, cd .. etc are supported
Relative and absolute path as arguments.
Updates PWD and OLDPWD environment variables.
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
