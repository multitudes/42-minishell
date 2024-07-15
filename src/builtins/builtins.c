/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 22:01:13 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 13:57:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "builtins.h"
#include "libft.h"
#include "scanner.h"
#include "environment.h"
#include "history.h"
#include <unistd.h>
#include "splash_error.h"
#include "utils.h"

static bool	execute_other_builtins(t_data *data, t_list *tokenlist, uint8_t *status)
{
	if (ft_strcmp(get_token_lexeme(tokenlist), "true") == 0)
	{
		*status = 0;
		return (true);
	}
	else if (ft_strcmp(get_token_lexeme(tokenlist), "false") == 0)
	{
		*status = 1;
		return (true);
	}
	else if (ft_strcmp(data->input, "history -c") == 0 \
			|| ft_strcmp(data->input, "history --clear") == 0)
	{
		clear_hist_file(data->homepath);
		rl_clear_history();
		*status = 0;
		return (true);
	}
	else if (ft_strcmp(data->input, "history") == 0)
	{
		*status = print_history(data->env_arr);
		return (true);
	}
	return (false);
}

/*
Function to call to execute minishell builtin functions + history, true, false.
Returns exit status of executed builtin.
*/
uint8_t	execute_builtin(t_list *tokenlist, t_data *data)
{
	uint8_t	status;

	status = 0;
	debug("Execute builtin. token lexeme: %s, ast type: %i", get_token_lexeme(tokenlist), data->ast->type);
	update_dollar_underscore(data->env_arr, data->ast->tokenlist);
	if (!tokenlist)
		return (EXIT_FAILURE);
	if (ft_strcmp(get_token_lexeme(tokenlist), "echo") == 0)
		return (execute_echo_builtin(tokenlist));
	else if (ft_strcmp(get_token_lexeme(tokenlist), "cd") == 0)
		return (execute_cd_builtin(data->env_arr, tokenlist));
	else if (ft_strcmp(get_token_lexeme(tokenlist), "pwd") == 0)
		return (execute_pwd_builtin());
	else if (ft_strcmp(get_token_lexeme(tokenlist), "export") == 0)
		return (execute_export_builtin(data->env_arr, tokenlist));
	else if (ft_strcmp(get_token_lexeme(tokenlist), "unset") == 0)
		return (execute_unset_builtin(data->env_arr, tokenlist));
	else if (ft_strcmp(get_token_lexeme(tokenlist), "env") == 0)
		return (execute_env_builtin(data->env_arr, tokenlist));
	else if (ft_strcmp(get_token_lexeme(tokenlist), "exit") == 0)
		return (execute_exit_builtin(data, tokenlist));
	else if (execute_other_builtins(data, tokenlist, &status))
		return (status);
	else if (get_token_type(tokenlist) == BUILTIN)
		return (stderr_and_status("builtin not implemented", 2));
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
Function merges the current and next token in a tokenlist:
- type of the first token gets preserved
- the token lexemes are joined together in a new lexeme
- the folldbyspace info from the second token is preserved
- the second token get deleted and the list node
pointed to the following node in the list
*/
int	merge_tokens(t_list *tokenlist)
{
	t_token	*token_1;
	t_token	*token_2;
	t_list	*tofree;
	char	*new_lexeme;

	debug("Token merge");
	token_1 = get_curr_token(tokenlist);
	token_2 = get_curr_token(tokenlist->next);
	tofree = tokenlist->next;
	new_lexeme = ft_strjoin(token_1->lexeme, token_2->lexeme);
	free(token_1->lexeme);
	free(token_2->lexeme);
	token_1->folldbyspace = token_2->folldbyspace;
	token_1->lexeme = new_lexeme;
	if (ft_strlen(token_1->lexeme) == 0)
		token_1->type = token_2->type;
	tokenlist->next = tokenlist->next->next;
	if (tokenlist->next)
		tokenlist->next->prev = tokenlist;
	free(token_2);
	free(tofree);
	if (tokenlist->next && !token_followed_by_space(tokenlist))
		merge_tokens(tokenlist);
	return (0);
}

bool	read_only_variable(const char *key)
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
