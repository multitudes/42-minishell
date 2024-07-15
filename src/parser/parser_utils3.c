/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:30:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 17:34:10 by lbrusa           ###   ########.fr       */
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

bool	get_new_tokenlist(t_list *expr_node, t_list **new_tokenlist, \
							bool *has_node)
{
	char	*newlexeme;

	newlexeme = ft_substr(get_token_lexeme(expr_node), 1, \
					ft_strlen(get_token_lexeme(expr_node)) - 2);
	*new_tokenlist = tokenizer(newlexeme);
	*has_node = tokenlist_has_astnode(*new_tokenlist);
	free(newlexeme);
	if (new_tokenlist == NULL)
		return (false);
	return (true);
}

/*
 * Used in the ast fuctions to create a terminal node
 * it returns a boolean to notify if I need to create a new node
 * in the ast tree the return value is true if the new_tokenlist 
 * contains a node like a pipe or a list && ||
 * new_tokenlist from the tokenizer is the new list of tokens
 * NOTE: I do not change head if there is a node before the expression
 * I dont take care of empty expressions or parenthesis
 */
bool	replace_expression_tokens(t_list **head, t_list **input_tokens)
{
	t_list	*expr_node;
	t_list	*new_tokenlist;
	bool	has_node;

	has_node = false;
	expr_node = *input_tokens;
	if (!get_new_tokenlist(expr_node, &new_tokenlist, &has_node))
		return (false);
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
	ft_lstdelone(expr_node, free_tokennode);
	return (has_node);
}

/*
 *I get a t_list node in input with my token as expression type
 *and want to substitute it with the content of the expression
 *it is like cut and paste a linked list
 *if I have an expression and cant replace it then I keep on running?
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
