/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:39:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 13:00:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
Creates ast node(s) from tokens.
Question: is tmp variable needed or could iteration be done directly on &input_tokens?

introducing a new node type for the tree, t_ast_node.
The tree will be composed of nodes, each node will have a type,
a left and a right node, and a list of tokens as a t_list.
*/
t_ast_node	*create_ast(t_list *tokenlist)
{
	t_ast_node	*rootnode;
	t_list		*tmp;

	rootnode = NULL;
	tmp = tokenlist;
	debug("in rootnode create_ast tokenlist :");
	if (tokenlist == NULL)
		return (NULL);
	while (tmp)
	{
		debug("in while loop create_ast tokenlist :%s", ((t_token *)tmp->content)->lexeme);
		rootnode = parse_list(&tmp);
		if (!rootnode)
			return (NULL);
	}
	return (rootnode);
}

/*
in this implementation the t_ast_node is a binary tree node
and I malloc the node but the token list is a pointer to 
the existing list I receive from the tokenizer.
Still I need to free it somewhere and since it will not
be needed later I free it here. Also the input_tokens list 
is split in pieces in the tree so it would not be possible
to traverse the list and free it later.
I use free_tokennode as a function to free because the token list 
contents are of type (void*)content and I need to cast it to
t_token* to free the lexeme string and the token itself. 
*/
void	*free_ast(t_ast_node **ast)
{
	debug("free_ast");
	if (ast == NULL || *ast == NULL)
		return (NULL);
	if ((*ast)->left)
		free_ast(&((*ast)->left));
	if ((*ast)->right)
		free_ast(&((*ast)->right));
	if ((*ast)->tokenlist) 
		ft_lstclear(&((*ast)->tokenlist), free_tokennode);
	free(*ast);
	*ast = NULL;
	debug("free_ast finished");
	return (NULL);
}

/* 
For debugging: print the ast tree  
Each node is printed with its type.
the content of each node in the linked list is a t_token like:

typedef struct	s_token {
	t_tokentype	type;
	char		*lexeme;
	int 		start;
}				t_token ;
typedef struct s_token t_token;
*/
void	print_ast(t_ast_node *a, int level)
{
	t_list	*tokenlist;
	t_token	*token;

	if (a == NULL)
		return ;
	tokenlist = a->tokenlist;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return ;
	while (tokenlist)
	{
		token = (t_token *)tokenlist->content;
		debug("level %d - token type: %d - lexeme %s", level, \
		(t_tokentype)(token->type), token->lexeme);
		tokenlist = tokenlist->next;
	}
	if (a->left)
		print_ast(a->left, ++level);
	if (a->right)
		print_ast(a->right, ++level);
}
