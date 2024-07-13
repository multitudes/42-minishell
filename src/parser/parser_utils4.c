/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:11:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 12:11:48 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
safely get the current token
*/
t_token	*get_curr_token(t_list *input_tokens)
{
	t_token	*token;

	if (!input_tokens)
		return (NULL);
	token = input_tokens->content;
	return (token);
}

/*
safely get the lexeme of the current token
*/
char	*get_token_lexeme(t_list *input_tokens)
{
	t_token	*token;

	if (!input_tokens || !input_tokens->content)
		return (NULL);
	token = input_tokens->content;
	return (token->lexeme);
}

/*
safely get the type of the current token
*/
t_tokentype	get_token_type(t_list *input_tokens)
{
	t_token	*token;

	if (!input_tokens || !input_tokens->content)
		return (NULL_TOKEN);
	token = input_tokens->content;
	if (!token->lexeme)
		return (NULL_TOKEN);
	return (token->type);
}