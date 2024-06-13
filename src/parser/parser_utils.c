/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:28:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/13 09:03:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"

/*
*/
t_ast_node	*new_node(t_nodetype type, t_ast_node *left, t_ast_node *right, \
t_list *token_list)
{
	t_ast_node	*node;

	if (token_list == NULL)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (null_on_err("malloc failed in new_node"));
	node->type = type;
	node->parent = NULL;
	node->left = left;
	node->right = right;
	node->token_list = token_list;
	if (left != NULL)
		left->parent = node;
	if (right != NULL)
		right->parent = node;
	return (node);
}

/*
Just utility function for debugging without have to check the 
input for null 
*/
void	print_token(t_list *input_token)
{
	t_token	*token;

	if (input_token == NULL)
		return ;
	token = (t_token *)input_token->content;
	if (token == NULL)
		return ;
	printf("token type: %d, %s", token->type, token->lexeme);
}

/*
 * count the number of nodes in the t_list
 * TODO not currently used!
*/
int	count_list(t_list *input_tokens)
{
	int		count;
	t_list	*tmp;

	count = 0;
	tmp = input_tokens;
	if (input_tokens == NULL)
		return (0);
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

/*
 * Since each node in the ast has a piece of the input_tokens linked list
 * I need to break the list at this node disconnecting from the previous list
*/
void	break_list(t_list **input_tokens)
{
	if (*input_tokens)
	{
		if ((*input_tokens)->prev)
		{
			(*input_tokens)->prev->next = NULL;
			(*input_tokens)->prev = NULL;
		}
	}
}
