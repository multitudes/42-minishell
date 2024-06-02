/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:24:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 21:14:58 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
peek wil look ahead to see if my string is beginning with sequence of chars
- input is the string to check
- identifier is the string to check for
- need_delim is true if the identifier must be followed by a space to be valid
ex '||' works without spaces at the end but 'echo' is valid with space only 
or with 
one of the metacharacters : ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
peek is case insensitive!
*/
bool	peek(const char *input, const char *identifier, bool need_delim)
{
	int	i;
	int	n;

	i = 0;
	n = ft_strlen(identifier);
	while (i < n && input[i] == identifier[i])
		i++;
	if (i == n && ((need_delim && is_delimiter(input[i])) || !need_delim))
		return (true);
	else
		return (false);
}

/*
creates a simple t_list node - the token is in the content of the node
in form of a string that will need to be freed
*/
t_list	*create_token(t_tokentype type, const char *lexeme, int *i)
{
	t_token	*token;
	t_list	*new_node;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->lexeme = ft_strdup(lexeme);
	*i = *i + ft_strlen(lexeme);
	debug("token created type %d -%s-", token->type, token->lexeme);
	new_node = ft_lstnew(token);
	if (new_node == NULL)
	{
		free_token(token);
		return (NULL);
	}
	return (new_node);
}

/*
this function checks if the tmp string is NULL and also if 
the malloc in create token fails and updates the error flag in the 
data struct so the loop will stop and free the data
it still returns true because when a token subfunction returns true
it means the token has been recognized and the scanner can move on
otherwise it would keep on looking for the token
* defensive programming. passing a string literal to add_token will
never fail but I still check for NULL lexemes!
*/
bool	add_token(t_mini_data *data, int *i, const char *lexem, int type)
{
	t_list	*token;

	if (!lexem)
		scanner_error(data, "error: empty lexem in token creation");
	else
	{
		token = create_token(type, lexem, i);
		if (token)
			ft_lstadd_back(&data->token_list, token);
		else
			scanner_error(data, "error: malloc in create_token failed");
	}
	return (true);
}

/*
condition is a pointer to a function that will be used to check the
character in the wjile loop. I might need is_digit for numbers or
is_alnum for identifiers etc
Thgis works for easy tokens.
*/
bool	process_token(t_mini_data *data, int *i, bool (*cnd)(char), int type)
{
	int		start;
	char	*tmp;

	start = *i;
	while (cnd(data->input[*i]))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start);
	add_token(data, &start, tmp, type);
	free(tmp);
	*i = start;
	return (true);
}
