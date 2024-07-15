/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:01:47 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/14 16:01:51 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "analyser.h"
#include "minishell.h"
#include "scanner.h"
#include "utils.h"
#include <libft.h>

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
In a token lexeme replaces all $-indicated variables
with their respective values or with empty string if not existing.
Returns the new lexeme, which must be freed.
*/
char	*replace_dollar_vars(t_data *data, char *lexeme)
{
	int		i;
	char	*new_lexeme;
	char	*key_value;
	char	*key;
	char	*temp[3];

	i = 0;
	new_lexeme = ft_strdup(lexeme);
	key = NULL;
	while (new_lexeme && new_lexeme[i])
	{
		key = get_key(new_lexeme + i);
		if (key != NULL)
		{
			if (ft_strlen(key) == 0)
				key_value = NULL;
			else if (ft_strcmp(key, "?") == 0 && data)
				key_value = ft_itoa(data->exit_status);
			else
				key_value = mini_get_env(data->env_arr, key);
			temp[0] = ft_strndup(new_lexeme, i);
			temp[1] = ft_strdup(new_lexeme + i + ft_strlen(key) + 1);
			temp[2] = new_lexeme;
			new_lexeme = ft_strjoin3(temp[0], key_value, temp[1]);
			i = i + ft_strlen(key_value);
			free(temp[0]);
			free(temp[1]);
			free(temp[2]);
			if (ft_strcmp(key, "?") == 0 && data)
				free (key_value);
			free(key);
			continue ;
		}
		i++;
	}
	return (new_lexeme);
}

/*
Returns true if string only has one '$' as char.
*/
bool	single_dollar(char *str)
{
	if (ft_strlen(str) == 1 && str[0] == '$')
		return (true);
	return (false);
}

/*
Used to expand variables indicated by "$".
*/
void	expand_dollar(t_data *data, t_token *token)
{
	char	*var;

	var = token->lexeme;
	if (!token || !token->lexeme)
		return ;
	if (single_dollar(token->lexeme))
	{
		token->type = EXPANDED;
		return ;
	}
	else if (ft_strcmp(token->lexeme, "$0") == 0)
		var = ft_strdup("splash");
	else if (ft_strchr(token->lexeme, '$'))
		var = replace_dollar_vars(data, token->lexeme);
	free(token->lexeme);
	token->lexeme = var;
	token->type = EXPANDED;
	debug("Expand dollar, new token: %s, type: %i", token->lexeme, token->type);
}

/*
Expands $? to exit status
*/
void	expand_exit_status(t_data *data, t_token *token)
{
	char	*temp;

	if (data)
		temp = ft_itoa(data->exit_status);
	else
		return ;
	if (token)
	{
		free(token->lexeme);
		token->lexeme = temp;
		token->type = WORD;
	}
	else
		free(temp);
}
