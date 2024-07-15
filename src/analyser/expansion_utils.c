/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:04:28 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/14 16:04:33 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "analyser.h"
#include "scanner.h"
#include "parser.h"
#include "darray.h"
#include "utils.h"

static char	*retrieve_home_from_cwd(char *cwd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cwd && cwd[i])
	{
		if (cwd[i] == '/')
			count++;
		i++;
		if (count == 3)
			break ;
	}
	if (count == 3 || (count == 2 && cwd[i] == '\0'))
		return (ft_strndup(cwd, i - 1));
	return (NULL);
}

/*
Get home from environment -  it is a tricky one
because bash always manages to get the home directory
even if HOME is not set in the environment
We are also not able to use special functions
so the best we can do is to check the user from 
the environment. In wtc/passwd the current user might also
not be included so it is not a viable option.
params:
env_array: if NULL or HOME is not set, get the home 
from the USER environment variable and collated with /home
Just pass NULL to the func ad you can get home from anywhere
but both need to be freed!
*/
char	*get_home(t_darray *env_arr)
{
	char	*home;
	char	buf[PATH_MAX];
	char	*cwd;

	home = mini_get_env(env_arr, "HOME");
	cwd = NULL;
	if (home && debug("HOME from env"))
		return (ft_strdup(home));
	else if (getenv("USER") && debug("home from USER"))
		return (ft_strjoin("/home/", getenv("USER")));
	else
	{
		cwd = getcwd(buf, PATH_MAX);
		if (cwd && count_char_in_str(cwd, '/') > 1 && debug("HOME from cwd"))
			home = retrieve_home_from_cwd(cwd);
	}
	if (home)
		return (home);
	debug("HOME manually set to /home");
	return (ft_strdup("/home"));
}

/*
Expands "~" in pathnames
*/
void	expand_path(t_darray *env_arr, t_list *tokenlist, t_exp_flags *flags)
{
	char	*lexeme;
	char	*home;
	t_token	*token;

	token = get_curr_token(tokenlist);
	if (!token)
		return ;
	home = get_home(env_arr);
	if (token->type == TILDE && flags->equal_status == 1 \
		&& valid_tilde_expansion(tokenlist, 0))
		lexeme = home;
	else
	{
		lexeme = replace_tilde_in_str(tokenlist, token->lexeme, home, flags);
		free(home);
	}
	token->type = WORD;
	free(token->lexeme);
	token->lexeme = lexeme;
	debug("Expanded path token: %s", token->lexeme);
}
