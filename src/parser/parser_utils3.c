/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:30:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 10:23:08 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

bool	tokenize_expression_token(t_list **input_tokens, t_list **expr_node, t_list **new_tokenlist, char **newlexeme)
{
	*expr_node = *input_tokens;
	*newlexeme = ft_substr(get_token_lexeme(*expr_node), 1, \
					ft_strlen(get_token_lexeme(*expr_node)) - 2);
	*new_tokenlist = tokenizer(*newlexeme);
	if (*new_tokenlist == NULL)
	{
		free(*newlexeme);
		return (false);
	}
	return (true);
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
	t_list	*new_tokenlist;
	char	*newlexeme;
	bool	has_node;

	if (!tokenize_expression_token(input_tokens, &expr_node, &new_tokenlist, &newlexeme))
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
	free(newlexeme);
	ft_lstdelone(expr_node, free_tokennode);
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
