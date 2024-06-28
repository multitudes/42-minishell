/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 22:50:53 by rpriess           #+#    #+#             */
/*   Updated: 2024/06/28 19:38:04 by lbrusa           ###   ########.fr       */
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

char *execute_getcwd(char old_dir[], char *message)
{
    char    *retval;

    retval = NULL;
    retval = getcwd(old_dir, PATH_MAX);
    if (!retval)
		perror(message);
    return (retval);
}

uint8_t execute_cd_tokenlist(t_darray *env_arr, t_list *tokenlist)
{
	if (tokenlist && get_token_lexeme(tokenlist))
	{
		if (ft_strncmp(get_token_lexeme(tokenlist), "-", 2) == 0)
		{
			if (!mini_get_env(env_arr, "OLDPWD"))
				return (print_minishell_error_status("cd: OLDPWD not set", 1));
			else
			{
				if (chdir(mini_get_env(env_arr, "OLDPWD")))
					return (status_and_perror("minishell: cd", 1));
				execute_pwd_builtin();
			}
		}
		else if (chdir(get_token_lexeme(tokenlist)))
            return (status_perror2("minishell: cd: ", get_token_lexeme(tokenlist), 1));
    }
    else
	{
		if (!mini_get_env(env_arr, "HOME"))
			return (print_minishell_error_status("cd: HOME not set", 1));
		else if (chdir(mini_get_env(env_arr, "HOME")))
			return (status_and_perror("minishell: cd", 1));
	}
    return (0);
}

/*
Function merges the current and next token in a tokenlist:
- type of the first token gets preserved
- the token lexemes are joined together in a new lexeme
- the folldbyspace info from the second token is preserved
- the second token get deleted and the list node pointed to the following node in the list
*/
int merge_tokens(t_list *tokenlist)
{
    t_token *token_1;
    t_token *token_2;
    t_list	*tofree;
    char    *new_lexeme;

    debug("Token merge");
    token_1 = get_curr_token(tokenlist);
    token_2 = get_curr_token(tokenlist->next);
    tofree = tokenlist->next;
    new_lexeme = ft_strjoin(token_1->lexeme, token_2->lexeme);
    free(token_1->lexeme);
    free(token_2->lexeme);
    token_1->folldbyspace = token_2->folldbyspace;
    token_1->lexeme = new_lexeme;
    tokenlist->next = tokenlist->next->next;
    if (tokenlist->next)
        tokenlist->next->prev = tokenlist;
    free(token_2);
	free(tofree);
    if (tokenlist->next && !token_followed_by_space(tokenlist))
        merge_tokens(tokenlist);
    return (0);
}

bool    read_only_variable(const char *key)
{
    if (ft_strncmp(key, "PPID", 5) == 0)
        return (true);
    else if (ft_strncmp(key, "EUID", 5) == 0)
        return (true);
    else if (ft_strncmp(key, "UID", 4) == 0)
        return (true);
    else
        return (false);
}

bool    no_valid_identifier(const char *key)
{
    int i;

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