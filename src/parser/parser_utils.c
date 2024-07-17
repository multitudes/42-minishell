/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:28:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 13:48:22 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "splash_error.h"

/*
*/
t_ast_node	*new_node(t_nodetype type, t_ast_node *left, t_ast_node *right, \
						t_list *tokenlist)
{
	t_ast_node	*node;

	if (tokenlist == NULL)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (perror_and_null("malloc new_node"));
	node->type = type;
	node->left = left;
	node->right = right;
	node->tokenlist = tokenlist;
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
