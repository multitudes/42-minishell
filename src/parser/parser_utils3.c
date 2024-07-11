/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:30:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/11 08:33:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
Checks if tokens in tokenlist are only of type flag or NULL.
Used for heredoc functionality to check 
for arguments of tokens of type COMMAND or TERMINAL.
*/

bool	only_flags(t_list *input_tokens)
{
	while (input_tokens)
	{
		if (get_token_type(input_tokens) != FLAGS)
			return (false);
		input_tokens = input_tokens->next;
	}
	return (true);
}

/*
JUST a check for the type of token - not currently used!
*/
bool	is_redirection(t_list *input_tokens)
{
	t_list	*tmp;
	t_token	*token;

	tmp = input_tokens;
	token = NULL;
	if (input_tokens == NULL)
		return (0);
	while (tmp)
	{
		token = (t_token *)tmp->content;
		if (token->type == REDIRECT_OUT || token->type == REDIRECT_IN || \
		token->type == DGREAT || token->type == DLESS)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/*
when I extract tokens from a parenthesis or expression
I need to check if there if the list has just normal tokens or
it will potentially create a node like pipe or a new list
because I will handle them differently
*/
bool	tokenlist_has_astnode(t_list *new_tokenlist)
{
	t_list	*tmp;

	tmp = new_tokenlist;
	if (new_tokenlist == NULL)
		return (false);
	while (tmp)
	{
		if (is_not_control_token(get_curr_token(tmp)))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

/*
I now take care of empty expressions in the scanner
NOTE: I do not change head if there is a node before the expression
new_tokenlist from the tokenizer is the new list of tokens
and if that is null I already have the scanner error message 
on stderr and do not need to print anothererror... I will continue?
return -1?
*/
bool	replace_expression_tokens(t_list **head, t_list **input_tokens)
{
	t_list	*expr_node;
	t_token	*curr_token;
	t_list	*new_tokenlist;
	bool	has_node;

	has_node = false;
	expr_node = *input_tokens;
	curr_token = get_curr_token(expr_node);
	new_tokenlist = tokenizer(ft_substr(curr_token->lexeme, 1, \
	ft_strlen(curr_token->lexeme) - 2));
	if (new_tokenlist == NULL)
		return (false);
	has_node = tokenlist_has_astnode(new_tokenlist);
	*input_tokens = (*input_tokens)->next;
	if (*input_tokens)
	{
		(*input_tokens)->prev = ft_lstlast(new_tokenlist);
		ft_lstlast(new_tokenlist)->next = *input_tokens;
	}
	if ((expr_node)->prev)
	{
		expr_node->prev->next = new_tokenlist;
		new_tokenlist->prev = expr_node->prev;
	}
	else
		*head = new_tokenlist;
	free(curr_token->lexeme);
	free(curr_token);
	free(expr_node);
	return (has_node);
}

/*
I get a t_list node in input with my token as expression type
and want to substitute it with the content of the expression
it is like cut and paste a linked list
if I have an expression and cant replace it then I keep on running?
*/
bool	extract_expression(t_list **head, t_list **input_tokens)
{
	if (get_token_type(*input_tokens) == EXPRESSION)
		return (replace_expression_tokens(head, input_tokens));
	return (false);
}

bool	is_not_control_token(t_token *token)
{
	if (token == NULL)
		return (false);
	if (token->type == PIPE || token->type == AND_IF || \
	token->type == OR_IF || token->type == PIPE_AND)
		return (false);
	return (true);
}
