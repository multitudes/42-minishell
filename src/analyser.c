/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:37:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 19:08:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "analyser.h"
#include "scanner.h"

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
	debug("which_ast_node");
	if (token->type == BUILTIN || token->type == TILDE)
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
		if (token->type == TILDE)
			temp = ft_strdup(token->lexeme = mini_get_env(data, "HOME"));
		else if (mini_get_env(data, key + 1))
			temp = ft_strdup(token->lexeme = mini_get_env(data, key + 1));
		else
			temp = NULL;
		free(key);
		token->lexeme = temp;
		token->type = WORD;
}

void	read_exit_status(t_data *data, t_token *token)
{
	char	*temp;

	debug("read_exit_status");
	temp = ft_itoa(data->exit_status);
	free(token->lexeme);
	token->lexeme = temp;
	token->type = WORD;
}

void	extract_string(t_token *token)
{
	debug("extract_string");
	ft_strlcpy(token->lexeme, (const char *)token->lexeme + 1, ft_strlen(token->lexeme) - 1);
	token->type = WORD;
}

void	expand_string(t_data *data, t_token *token)
{
	char	*old_lexeme;
	t_list	*string_tokens;
	t_list	*ptr_to_list;
	char	*temp_lexeme;

	debug("expand_string");
	old_lexeme = ft_strtrim(token->lexeme, "\"");
	if (!ft_strchr(old_lexeme, '$'))
	{
		token->lexeme = old_lexeme;
		return ;
	}
	string_tokens = string_tokenizer((const char *)old_lexeme);
	ptr_to_list = string_tokens;
	free(token->lexeme);
	token->lexeme = NULL;
	while (string_tokens)
	{
		if (((t_token *)string_tokens->content)->type == VAR_EXPANSION)
			expand_variable(data, (t_token *)string_tokens->content);
		if (((t_token *)string_tokens->content)->type == TILDE)
			expand_variable(data, (t_token *)string_tokens->content);
		else if (((t_token *)string_tokens->content)->type == DOLLAR_QUESTION)
			read_exit_status(data, (t_token *)string_tokens->content);
		temp_lexeme = token->lexeme;
		token->lexeme = ft_strjoin((const char *)temp_lexeme, ((t_token *)string_tokens->content)->lexeme);
		free(temp_lexeme);
		string_tokens = string_tokens->next;
	}
	ft_lstclear(&ptr_to_list, free_token); //free_token function is from scanner.h
	token->type = WORD;
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
	if (ast == NULL)
		return ;
	debug("analyse expand");
	debug("Received token type: %d ast node type: %d lexeme: %s", ((t_token *)token_list->content)->type, ast->type, ((t_token *)token_list->content)->lexeme);
	// assignng a ast node type to the node
	which_ast_node(ast);
	while (token_list)
	{
		token = token_list->content;
		if (token->type == VAR_EXPANSION || token->type == TILDE)
			expand_variable(data, token);
		else if (token->type == DOLLAR_QUESTION)
			read_exit_status(data, token);
		else if (token->type == S_QUOTED_STRING)
			extract_string(token);
		else if (token->type == QUOTED_STRING)
			expand_string(data, token);
		debug("Expanded token type: %d ast node type: %d lexeme: %s", token->type, ast->type, token->lexeme);
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
