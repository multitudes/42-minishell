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

void	expand_variable(t_data *data, t_token *token)
{
		char	*key;
		char	*temp;

		debug("expand_variable");
		key = token->lexeme;
		if (mini_get_env(data, key + 1))
			temp = ft_strdup(token->lexeme = mini_get_env(data, key + 1));
		else
			temp = NULL;
		free(key);
		token->lexeme = temp;
}

void	read_exit_status(t_data *data, t_token *token)
{
	char	*temp;

	debug("read_exit_status");
	temp = ft_itoa(data->exit_status);
	free(token->lexeme);
	token->lexeme = temp;
}

void	extract_string(t_token *token)
{
	debug("extract_string");
	ft_strlcpy(token->lexeme, (const char *)token->lexeme + 1, ft_strlen(token->lexeme) - 1);
}
/*
the function of the analyser is to walk on the tree and analyze and expand 
the nodes that need to.
Required is expansion of env variables (incl $PATH), of local variables and of $?
Optional are the ~, $!, etc..)
" " ' ' 
Also optional appear to be $(..) ${..} $'..' $".."
I replicate here the code for the print_ast function which is in parser.c
and walks the tree. The idea is to walk through each node... expand
and then pass it to the executor
*/
void analyse_expand(t_ast_node *ast, t_data *data)
{
	t_list	*token_list;
	t_token	*token;

	//(void)data;
	token_list = ast->token_list;
	token = token_list->content; //assignment here only needed for debugging
	if (ast == NULL)
		return ;
	debug("analyse expand");
	debug("token type: %d ast node type: %d lexeme: %s", token->type, ast->type, token->lexeme);
	// assignng a ast node type to the node
	// which_ast_node(ast);
	while (token_list)
	{
		token = token_list->content;
		if (token->type == VAR_EXPANSION && ast->type == NODE_TERMINAL)
			expand_variable(data, token);
		if (token->type == DOLLAR_QUESTION)
			read_exit_status(data, token);
		if (token->type == S_QUOTED_STRING)
			extract_string(token);
		//if (token->type == QUOTED_STRING)
		debug("token type: %d ast node type: %d lexeme: %s", token->type, ast->type, token->lexeme);
		token_list = token_list->next;
	}
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
