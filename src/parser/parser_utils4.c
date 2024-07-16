/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:11:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 14:56:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * safely get the current token
 */
t_token	*get_curr_token(t_list *tokenlist)
{
	t_token	*token;

	if (!tokenlist)
		return (NULL);
	token = tokenlist->content;
	return (token);
}

/*
 * safely get the lexeme of the current token
 */
char	*get_token_lexeme(t_list *tokenlist)
{
	t_token	*token;

	if (!tokenlist || !tokenlist->content)
		return (NULL);
	token = tokenlist->content;
	return (token->lexeme);
}

/*
 * safely get the type of the current token
 */
t_tokentype	get_token_type(t_list *tokenlist)
{
	t_token	*token;

	if (!tokenlist || !tokenlist->content)
		return (NULL_TOKEN);
	token = tokenlist->content;
	if (!token->lexeme)
		return (NULL_TOKEN);
	return (token->type);
}
