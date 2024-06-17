/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:17:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 14:10:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
follows the grammar
*/
t_ast_node	*parse_terminal(t_list **input_tokens)
{
	t_ast_node	*a;
	t_list		*head;
	bool		expr_has_node;

	a = NULL;
	expr_has_node = false;
	head = *input_tokens;
	if (input_tokens == NULL || *input_tokens == NULL)
		return (NULL);
	while (is_not_control_token(get_curr_token(*input_tokens)))
	{
		if (extract_expression(&head, input_tokens))
			expr_has_node = true;
		if (head == NULL)
			return (NULL);
		if (input_tokens == NULL || *input_tokens == NULL)
			break ;
		if (is_not_control_token(get_curr_token(*input_tokens)))
			*input_tokens = (*input_tokens)->next;
	}
	break_list(input_tokens);
	if (expr_has_node)
		return (parse_list(&head));
	else
		a = new_node(NODE_TERMINAL, NULL, NULL, head);
	if (head->prev)
		head->prev = NULL;
	return (a);
}

bool	is_pipe_token(t_list *input_tokens)
{
	t_token	*token;

	if (input_tokens == NULL)
		return (false);
	token = (t_token *)input_tokens->content;
	if (token->type == PIPE || token->type == PIPE_AND)
		return (true);
	return (false);
}

t_ast_node	*parse_pipeline(t_list **token_list)
{
	t_ast_node	*a;
	t_token		*token;
	t_ast_node	*b;

	a = NULL;
	token = NULL;
	if (token_list == NULL || *token_list == NULL)
		return (NULL);
	a = parse_terminal(token_list);
	if (a == NULL)
		return (NULL);
	while (is_pipe_token(*token_list))
	{
		token = get_curr_token(*token_list);
		if (!consume_token(token_list))
			return (NULL);
		b = parse_terminal(token_list);
		if (b == NULL)
			return (free_ast(&a));
		a = new_node(NODE_PIPELINE, a, b, ft_lstnew(token));
		if (a == NULL)
			return (free_ast(&a));
	}
	return (a);
}

/*
Parsing sequence of pipelines separated by operators "&&", "||"
*/
t_ast_node	*parse_list(t_list **token_list)
{
	t_token		*token;
	t_ast_node	*a;
	t_ast_node	*b;

	a = NULL;
	b = NULL;
	token = NULL;
	if (token_list == NULL || *token_list == NULL)
		return (NULL);
	a = parse_pipeline(token_list);
	if (a == NULL)
		return (NULL);
	while (*token_list)
	{
		token = get_curr_token(*token_list);
		if (token->type == AND_IF || token->type == OR_IF || \
		token->type == EXPRESSION)
		{
			if (token->type != EXPRESSION && !consume_token(token_list))
				return (NULL);
			b = parse_pipeline(token_list);
			if (b == NULL)
				return (free_ast(&a));
			a = new_node(NODE_LIST, a, b, ft_lstnew(token));
			if (a == NULL)
				return (free_ast(&a));
		}
		else if (*token_list)
		{
			debug("extraneus tken: %d, %s", \
			get_token_type(*token_list), \
			get_token_lexeme(*token_list));
			*token_list = (*token_list)->next;
		}
	}
	return (a);
}
