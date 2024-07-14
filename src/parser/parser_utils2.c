/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:30:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 19:23:40 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "scanner.h"
#include <libft.h>

/*
returns the next token after deleting the current token
and connecting the previous and next token
TODO check for leaks
*/
bool	consume_token_and_connect(t_list **input_tokens)
{
	t_list	*ptr_to_next;

	debug("consume token and connect, token to delete: %s", \
				((t_token *)((*input_tokens)->content))->lexeme);
	ptr_to_next = (*input_tokens)->next;
	if (*input_tokens == NULL)
		return (false);
	if ((*input_tokens)->prev)
		(*input_tokens)->prev->next = (*input_tokens)->next;
	if ((*input_tokens)->next)
		(*input_tokens)->next->prev = (*input_tokens)->prev;
	free((void *)((t_token *)((*input_tokens)->content))->lexeme);
	free((*input_tokens)->content);
	(*input_tokens)->content = NULL;
	free(*input_tokens);
	*input_tokens = ptr_to_next;
	if (!input_tokens || !*input_tokens)
		return (false);
	return (true);
}

/*
 * returns the next token but also breaks the list before the node 
 * performing the necessary checks
*/
bool	movetonexttoken_andbreak(t_list **input_tokens)
{
	if (*input_tokens == NULL)
		return (false);
	*input_tokens = (*input_tokens)->next;
	break_list(input_tokens);
	if (!input_tokens || !*input_tokens)
		return (false);
	return (true);
}

bool	token_followed_by_space(t_list *input_tokens)
{
	t_token	*token;

	if (!input_tokens || !input_tokens->content)
		return (false);
	token = input_tokens->content;
	return (token->folldbyspace);
}
