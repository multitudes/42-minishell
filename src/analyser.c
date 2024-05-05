/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:37:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/05 11:45:02 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
typedef struct s_ast_node {
    t_nodetype type;
    struct s_ast_node* left;
    struct s_ast_node* right;
	// for now not used
    // char** args;
	t_list *token_list;
} t_ast_node;

struct s_token {
	t_tokentype	type;
	char		*lexeme;
	int 		start;
};
typedef struct s_token t_token;

	tmp = ast->token_list;
	if (ast->type == NODE_EXPRESSION)
	{
		debug("NODE_EXPRESSION\n");
		while (tmp)
		{
			debug("-> %s\n", (char*)tmp->content);
*/
void	which_ast_node(t_ast_node *ast)
{
	t_list *tokenlist;
	t_token *token;

	tokenlist = ast->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return ;
	token = (t_token *)tokenlist->content;
	// debug("which_ast_node");
	// print_ast(ast);
	// check if the token list is a builtin
	debug("token type: %d", (t_tokentype)(token->type));
	debug("lexeme %s", (char *)(token->lexeme));
	if (token->type == BUILTIN)
	{
		ast->type = NODE_BUILTIN;
		debug("NODE_BUILTIN");
		return ;
	}
	// provisionally
	else if (token->type == PATHNAME || token->type == IDENTIFIER)
	{
		ast->type = NODE_COMMAND;
		debug("NODE_COMMAND");
		return ;
	}
	else
		debug("not TERMINAL");	
}
/*
the function of the analyser is to walk on the tree and analyze and expand 
the nodes that need to.
ex (.. expand the env variables, the ~, the $?, the $!, the $PATH, etc..)
(.. ) $(..) ${..} $'..' $".."
I replicate here the code for the print_ast function which is in parser.c
and walks the tree. The idea is to expand each node...
and then pass it to the executor
*/
void analyse_expand(t_ast_node *ast, t_mini_data *data)
{
	if (ast == NULL)
		return ;
	debug("\nanalyse expand");
	// assing a ast node type to the node
	which_ast_node(ast);

	// analyze node
	// expand any lexemes that need to be expanded

	// go to the next nodes and repeat
	debug("---------left -----------");
	if (ast->left)
		analyse_expand(ast->left, data);
	else
		debug("left is NULL");
	debug("---------right -----------");
	if (ast->right)
		analyse_expand(ast->right, data);
	else
		debug("right is NULL");
}
