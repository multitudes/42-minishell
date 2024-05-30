/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:30:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/30 09:49:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
This function is used to consume a token from the input_tokens list but also
needs to break the list before the node 
*/
t_token	*consume_token(t_list **input_tokens)
{
	t_token	*token;

	if (*input_tokens == NULL)
		return (NULL);
	token = (t_token *)(*input_tokens)->content;
	*input_tokens = (*input_tokens)->next;
	break_list(input_tokens);
	if (!input_tokens || !*input_tokens)
		return (NULL);
	return (token);
}

/*
safely get the current token
*/
t_token	*get_curr_token(t_list *input_tokens)
{
	t_token	*token;

	if (input_tokens == NULL)
		return (NULL);
	token = (t_token *)input_tokens->content;
	return (token);
}

/*
safely get the lexeme of the current token
*/
char	*get_token_lexeme(t_list *input_tokens)
{
	t_token	*token;

	if (input_tokens == NULL)
		return (NULL);
	token = (t_token *)input_tokens->content;
	return (token->lexeme);
}

/*
safely get the type of the current token
*/
t_tokentype	get_token_type(t_list *input_tokens)
{
	t_token	*token;

	if (input_tokens == NULL)
		return (0);
	token = (t_token *)input_tokens->content;
	return (token->type);
}
