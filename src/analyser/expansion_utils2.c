/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 09:22:07 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 13:14:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "utils.h"
#include "analyser.h"
#include <libft.h>

void	set_flags(t_list *tokenlist, t_exp_flags *flags)
{
	if (get_token_type(tokenlist) == VAR_EXPANSION)
		flags->starts_var_exp = true;
	if (ft_strchr(get_token_lexeme(tokenlist), '=') && flags->equal_status == 0)
		flags->equal_status = 1;
	else if (ft_strchr(get_token_lexeme(tokenlist), '=') \
				&& flags->equal_status == 1)
		flags->equal_status = 2;
}

void	reset_flags(t_exp_flags *flags)
{
	flags->equal_status = 1;
	flags->starts_var_exp = false;
}

/*
 * Checks ast node is a builtin or a command.
*/
void	which_ast_node(t_ast_node *ast)
{
	t_list	*tokenlist;

	tokenlist = ast->tokenlist;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return ;
	if (get_token_type(tokenlist) == BUILTIN)
		ast->type = NODE_BUILTIN;
	else if (get_token_type(tokenlist) == PATHNAME || \
			get_token_type(tokenlist) == WORD || \
			get_token_type(tokenlist) == COMMAND)
		ast->type = NODE_COMMAND;
	return ;
}

/*
Looks at next char in str. If it is a $,
then the potential key/var-name is returned.
'$' followed by invalid var-syntax, returns an empty string.
Returned key-string needs to be freed.
*/
char	*get_key(char *str)
{
	char	*end;

	if (!str)
		return (NULL);
	if (*str != '$')
		return (NULL);
	end = str + 1;
	if (end && (*end == ' ' || *end == '\0'))
		return (NULL);
	if (end && (*end == '?') && (*(end + 1) == '\0' \
			|| ((end + 1) && *(end + 1) == ' ')))
		return (ft_strdup("?"));
	if (end && (ft_isalnum(*end) || *end == '_'))
		end++;
	else
		return (ft_strdup(""));
	while (end && (ft_isalnum(*end) || *end == '_'))
		end++;
	return (ft_strndup(str + 1, (size_t)(end - (str + 1))));
}

/*
Gets the key value for a specified key from the env variable
for the purpose of $ expansion.
Key value is malloced and needs to be freed.
*/
char	*get_key_value(t_data *data, char *key)
{
	char	*key_value;

	if (ft_strlen(key) == 0)
		key_value = NULL;
	else if (ft_strcmp(key, "?") == 0 && data)
		key_value = ft_itoa(data->exit_status);
	else if (ft_strcmp(key, "0") == 0)
		key_value = ft_strdup("splash");
	else
		key_value = ft_strdup(mini_get_env(data->env_arr, key));
	return (key_value);
}
