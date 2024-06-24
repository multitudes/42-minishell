/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:30:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/06 12:22:30 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
returns the next token after deleting the current token
and connecting the previous and next token
TODO check for leaks
*/
bool	consume_token_and_connect(t_list **input_tokens)
{
	t_list	*tofree;

	debug("consume token and connect");
	tofree = *input_tokens;
	debug("token (lexeme) to delete: %s", get_token_lexeme(*input_tokens));
	if (*input_tokens == NULL)
		return (false);
	if ((*input_tokens)->prev)
		(*input_tokens)->prev->next = (*input_tokens)->next;
	if ((*input_tokens)->next)
		(*input_tokens)->next->prev = (*input_tokens)->prev;
	*input_tokens = (*input_tokens)->next;
	debug("next token (lexeme): %s", get_token_lexeme(*input_tokens));
	ft_lstdelone(tofree, free_tokennode);
	if (!input_tokens || !*input_tokens)
		return (false);
	return (true);
}

/*
returns the next token but also breaks the list before the node 
performing the necessary checks
TODO check for leaks
*/
bool	consume_token_and_break(t_list **input_tokens)
{
	t_list	*tofree;

	tofree = *input_tokens;
	if (*input_tokens == NULL)
		return (false);
	*input_tokens = (*input_tokens)->next;
	break_list(input_tokens);
	ft_lstdelone(tofree, free_tokennode);
	if (!input_tokens || !*input_tokens)
		return (false);
	return (true);
}

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
	return (token->type);
}

bool	token_followed_by_space(t_list *input_tokens)
{
	t_token	*token;

	if (!input_tokens || !input_tokens->content)
		return (false);
	token = input_tokens->content;
	return (token->folldbyspace);
}
