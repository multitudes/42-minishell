/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:01:47 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 12:29:13 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "scanner.h"
#include "utils.h"
#include "analyser.h"
#include <libft.h>

/*
Replaces a char + given key at position i by key_value
and returns the new lexeme.
*/
static char	*replace_lexeme(char *lexeme, char *key, char *key_value, int i)
{
	char	*new_lexeme;
	char	*front;
	char	*back;

	front = ft_strndup(lexeme, i);
	back = ft_strdup(lexeme + i + ft_strlen(key) + 1);
	new_lexeme = ft_strjoin3(front, key_value, back);
	free(front);
	free(back);
	free(lexeme);
	return (new_lexeme);
}

/*
In a token lexeme replaces all $-indicated variables
with their respective values or with empty string if not existing.
Returns the new lexeme, which must be freed.
*/
char	*replace_dollar_vars(t_data *data, char *lexeme)
{
	int		i;
	char	*key;
	char	*key_value;
	char	*new_lexeme;

	i = 0;
	key = NULL;
	key_value = NULL;
	new_lexeme = ft_strdup(lexeme);
	while (new_lexeme && new_lexeme[i])
	{
		key = get_key(new_lexeme + i);
		if (key != NULL)
		{
			key_value = get_key_value(data, key);
			new_lexeme = replace_lexeme(new_lexeme, key, key_value, i);
			i = i + ft_strlen(key_value);
			free(key_value);
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
