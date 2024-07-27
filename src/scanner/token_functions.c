/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:24:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/22 12:02:00 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "debug.h"
#include "libft.h"

/*
 * peek wil look ahead to see if my string is beginning with a sequence 
 * of chars that I want to recognize as a token.
 * the parameters are:
 * - input is the string to check
 * - identifier is the string to check for
 * - need_delim is true/EXACT if the identifier must be delimited by a 
 * end of word char to be valid, else is false/FUZZY if allows to be continued 
 * by other chars
 * ex '||' is FUZZY, works with/without spaces at the end but 'echo' 
 * is EXACT, works with valid with delim char.
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
 */
char	*return_lexeme_malloc(const char *lex)
{
	const char	*oldlex;
	char		*newlex;
	char		*result;

	if (!lex)
		return (NULL);
	result = ft_calloc(ft_strlen(lex) + 1, sizeof(char));
	if (!result)
		return (NULL);
	oldlex = lex;
	newlex = result;
	while (*oldlex)
	{
		if (*oldlex == '\\' && *(oldlex + 1) == '\\')
		{
			*newlex++ = *oldlex;
			oldlex += 2;
		}
		else
			*newlex++ = *oldlex++;
	}
	*newlex = '\0';
	return (result);
}

/*
 * creates a simple t_list node - the token is in the content of the node
 * in form of a string that will need to be freed
 */
t_list	*new_toknode(t_tokentype type, const char *lexeme, int *i, \
						bool folldbyspace)
{
	t_token	*token;
	t_list	*new_node;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->folldbyspace = folldbyspace;
	token->howmany = 0;
	token->lexeme = return_lexeme_malloc(lexeme);
	*i = *i + ft_strlen(token->lexeme);
	new_node = ft_lstnew(token);
	if (new_node == NULL)
	{
		free_tokennode(token);
		return (NULL);
	}
	return (new_node);
}

/*
 * this function checks if the tmp string is NULL and also if 
 * the malloc in create token fails and updates the error flag in the 
 * data struct so the loop will stop and free the data
 * it still returns true because when a token subfunction returns true
 * it means the token has been recognized and the scanner can move on
 * otherwise it would keep on looking for the token
 * * defensive programming. passing a string literal to add_token will
 * never fail but I still check for NULL lexemes!
 */
bool	add_token(t_mini_data *data, int *i, const char *lexem, \
					t_tokentype type)
{
	t_list	*token;

	if (!lexem)
		scanner_error(data, "splash: error: empty lexem in token creation");
	else
	{
		token = new_toknode(type, lexem, i, false);
		if (token)
			ft_lstadd_back(&data->tokenlist, token);
		else
			scanner_error(data, \
							"splash: error: malloc in new_toknode failed");
	}
	return (true);
}

/*
 * cnd - condition, is a pointer to a function that will be used to check the
 * character in the while loop. I might need is_digit for numbers or
 * is_alnum for identifiers etc
 * This works for easy tokens.
 */
bool	process_token(t_mini_data *data, int *i, bool (*cnd)(char), \
						t_tokentype type)
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
