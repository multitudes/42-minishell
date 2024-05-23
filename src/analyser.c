/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:37:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/18 12:21:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
it checks if the terminal ast node is a builtin or a command
but this could be already done in the parser 🧐🤨
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
	// debug("token type: %d", (t_tokentype)(token->type));
	// debug("lexeme %s", (char *)(token->lexeme));
	if (token->type == BUILTIN)
	{
		ast->type = NODE_BUILTIN;
		debug("NODE_BUILTIN");
		return ;
	}
	// provisionally
	else if (token->type == PATHNAME || token->type == WORD)
	{
		ast->type = NODE_COMMAND;
		debug("NODE_COMMAND");
		return ;
	}
	else if (token->type == TRUETOK || token->type == FALSETOK)
	{
		debug("NODE_TRUE or NODE_FALSE");
	}
	else
		debug("not TERMINAL");
}

/*
the function of the analyser is to walk on the tree and analyze and expand 
the nodes that need to.
need to expand the env variables, the ~, the $?, the $!, the $PATH, etc..)
(.. ) $(..) ${..} $'..' $".."
I replicate here the code for the print_ast function which is in parser.c
and walks the tree. The idea is to walk through each node... expand
and then pass it to the executor
*/
void analyse_expand(t_ast_node *ast, t_data *data)
{
	(void)data;
	if (ast == NULL)
		return ;
	debug("analyse expand");
	// assignng a ast node type to the node
	 which_ast_node(ast);
	//if (ast->type == VAR_EXPANSION)
	//{
		//remove "$"
		//check if followed by "?", then replace ast->tokenlist->content with exit status of last command
		//else search for string contained in tokenlist content in ENV_ARRAY before "="
		//if match, replace this string with string behind "="
	//}
	// analyze node
	// expand any lexemes that need to be expanded

	// go to the next nodes and repeat
	// debug("---------left -----------");
	// if (ast->left)
	// 	analyse_expand(ast->left, data);
	// else
	// 	debug("left is NULL");
	// debug("---------right -----------");
	// if (ast->right)
	// 	analyse_expand(ast->right, data);
	// else
	// 	debug("right is NULL");
}
