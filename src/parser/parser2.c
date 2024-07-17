/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:17:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 14:52:02 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

/*
 * function called in parse list
 */
void	*parse_terminal_inner_loop(t_list **head, t_list **input_tokens, \
										bool *expr_has_node)
{
	if (input_tokens == NULL || *input_tokens == NULL)
		return (NULL);
	while (is_not_control_token(*input_tokens))
	{
		if (extract_expression(head, input_tokens))
			*expr_has_node = true;
		if (*head == NULL)
			return (NULL);
		if (input_tokens == NULL || *input_tokens == NULL)
			break ;
		if (is_not_control_token(*input_tokens))
			*input_tokens = (*input_tokens)->next;
	}
	if (is_tree_control_token(*head))
	{
		ft_lstclear(head, free_tokennode);
		return (NULL);
	}
	return (*head);
}

/*
 * follows the grammar
 */
t_ast_node	*parse_terminal(t_list **input_tokens)
{
	t_ast_node	*a;
	t_list		*head;
	bool		expr_has_node;

	a = NULL;
	expr_has_node = false;
	head = *input_tokens;
	if (!parse_terminal_inner_loop(&head, input_tokens, &expr_has_node))
		return (NULL);
	break_list(input_tokens);
	if (expr_has_node)
		return (parse_list(&head));
	else
		a = new_node(NODE_TERMINAL, NULL, NULL, head);
	if (head->prev)
		head->prev = NULL;
	return (a);
}
