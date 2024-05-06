/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:39:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/05 18:02:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*

grammar:

// should i add this cases to the statement? | "<" | ">" | "2>" | "&>" | "<<"
// since they can come up like ls -la Makefile | grep fi > hey
// but it seems that when using input redirection it does not go to the first 
// pipe but to the one on its left: ex
//cat  | grep fi < file 
// will output the result of grep taking input from file ... so the bood is wrong (
	// the one in the link Chapter5-WritingYourOwnShell.pdf !)
// cat  | grep fie > hey  < file
// and same here cat will not get file as input... but grep does 
// here cat does
 // cat  > hey  < file

once the scanner get the list of lexemes i get a command table
ls -la a*
grep $pa
in:dflt out:file1 rr:dflt

this table will use wildcard expansion and variable substitution

to get the final table
list      -> pipeline ((";" | "&" | "&&" | "||") pipeline)* [";"] ["&"] ["\n"];
pipeline ->  expression  (("|" | "|&" | ";" | "&&" | "||" )expression)* ;

expression ->  	  command 
				| builtin 
				| heredoc 
				| redirection
				| [time [-p]] [!] expression
				| "(" statement ")";

command -> name (args)* ;
builtin -> name (args)* ; 
redirection -> expression ( "<" | ">" | ">>" | ">>&" | "2>" | "&> | &>> | 2>> | <> | >|") expression; 
heredoc -> 	expression "<<" delimiter;

delimiter -> STRING;
content -> MULTIPLE_LINE_TEXT;

name -> IDENTIFIER | COM_EXPANSION | VAR_EXPANSION;
args -> FLAGS | IDENTIFIER | STRING | QUOTED_STRING | SIMPLE_QUOTED_STRING | VAR_EXPANSION | EXPR_EXPANSION;


ex
(make &)  | wc -l
make &


GitHub Copilot
The redirection operators you've listed are the most commonly used in bash and similar Unix-like shells. Here's a brief description of each:

<: Redirects standard input from a file.
>: Redirects standard output to a file, overwriting the file if it exists.
>>: Redirects standard output to a file, appending to the file if it exists.
>>&: Redirects both standard output and standard error to a file, appending to the file if it exists.
2>: Redirects standard error to a file, overwriting the file if it exists.
&>: Redirects both standard output and standard error to a file, overwriting the file if it exists.
There are a few more redirection operators that you might encounter:

2>>: Redirects standard error to a file, appending to the file if it exists.
<>: Opens a file for both reading and writing.
>|: Redirects standard output to a file, overwriting the file even if the noclobber option has been set in the shell.
&>>: Redirects both standard output and standard error to a file, appending to the file if it exists.


=======================================
to recap :
statement -> expression (("|" | "|&" | ";")expression* + "&"? ; : 
A statement is an expression followed by zero or more expressions 
separated by a pipe (|), pipe and (|&), or semicolon (;). 
The statement can optionally end with an ampersand (&), 
which would run the command in the background.

expression -> command | builtin | heredoc | redirection | "(" statement ")"; : 
An expression can be a command, a builtin command, a heredoc, a redirection, 
or a statement enclosed in parentheses.


In this file
we will implement a parser method called "recursive descent,"
which is a top-down parser 

So the root is the first pipe or pipe_and operator..

*/
t_ast_node* new_node(t_nodetype type, t_ast_node* left, t_ast_node* right, t_list *expr_token_list)
{
	if (expr_token_list == NULL)
		return (NULL);
	t_ast_node *node = malloc(sizeof(t_ast_node));
	if (node == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	node->type = type;
	node->parent = NULL;
	node->left = left;
	node->right = right;
	node->token_list = expr_token_list;
	if (left != NULL && right != NULL)
	{
		left->parent = node;
		right->parent = node;
	}
	return (node);
}
/*
I will start to implement three types of redirection
    REDIRECT_OUT, // '>'
    REDIRECT_IN, // '<'
    APPEND, // '>>'
so if a node is not an expression I will check if a redirection or heredoc
	HEREDOC // <<
*/
bool	is_redirection(t_list *input_tokens)
{
	t_list *tmp;
	t_token *token;
	
	tmp = input_tokens;
	token = NULL;
	if (input_tokens == NULL)
		return (0);		
	while (tmp)
	{
		token = (t_token *)tmp->content;
		// check for redirections
		if (token->type == REDIRECT_BOTH || \
		token->type == REDIRECT_OUT || token->type == REDIRECT_IN || \
		token->type == APPEND || token->type == HEREDOC)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/*
introducing a new node type for the tree, t_ast_node.
The tree will be composed of nodes, each node will have a type,
a left and a right node, and a list of tokens as a t_list.
*/
t_ast_node *create_ast(t_mini_data *data, t_list *input_tokens)
{
	t_list *expr_token_list;
	t_ast_node *node;
	t_ast_node *left;
	t_ast_node *right;
	t_list *tmp;
	t_token *token;
	
	tmp = input_tokens;
	token = NULL;
	expr_token_list = NULL;
	if (input_tokens == NULL)
		return (NULL);		
	while (tmp)
	{
		token = (t_token *)tmp->content;
		// debug("token type: %d, %s", token->type, token->lexeme);
		if (token->type == PIPE || token->type == PIPE_AND)
		{
			debug("PIPE");
			if (is_redirection(expr_token_list))
				left = create_ast(data, expr_token_list);
			else 
				left = new_node(NODE_TERMINAL, NULL, NULL, expr_token_list);
			if (left == NULL)
			{
				debug("left is NULL");	
				return (NULL);
			}
			right = create_ast(data, tmp->next);
			if (right == NULL)
				return (NULL);
			node = new_node(NODE_PIPE, left, right, ft_lstnew(token));
			// free the token list ? not here because the pointer is used in the tree!
			// ft_lstclear(&expr_token_list, free);
			return (node);
		}
		// as long as I dont find a | or |& I will add the token to the list
		ft_lstadd_back(&expr_token_list, ft_lstnew(token));
		tmp = tmp->next;
	}
	// chedck for the redirections if I do not have any pipe
	// here I need to traverse my list again to check for redirections
	input_tokens = expr_token_list;
	expr_token_list = NULL;
	tmp = input_tokens;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		debug("check redir token type: %d, %s", token->type, token->lexeme);
		/*
		we dont implement all of them just the > but we can add the others
		    REDIRECT_OUT, // '>'
			REDIRECT_BOTH, // '&>'
			REDIRECT_FD, // '>&'
			REDIRECT_ERR, // '2>'
			REDIRECT_ERR_FD, //2>&
			to the parser
		*/
		if (token->type == REDIRECT_OUT || token->type == REDIRECT_BOTH || 
		token->type == REDIRECT_FD || token->type == REDIRECT_ERR || \
		token->type == REDIRECT_ERR_FD)
		{
			debug("REDIRECT_OUT");
			left = new_node(NODE_TERMINAL, NULL, NULL, expr_token_list);
			if (left == NULL)
			{
				debug("left is NULL");	
				return (NULL);
			}
			right = create_ast(data, tmp->next);
			if (right == NULL)
			{
				debug("right is NULL");
				return (NULL);
			}
			node = new_node(NODE_EXPRESSION, left, right, ft_lstnew(token));
			// free the token list?
			// ft_lstclear(&expr_token_list, free);
			return (node);
		}
		// as long as I dont find a << or >>  or < or > I will add the token to the list
		ft_lstadd_back(&expr_token_list, ft_lstnew(token));
		tmp = tmp->next;
	}
	// I am here. my token list has no pipe and no redirection and no heredocs
	// if I get here it is my terminal the leaf...?
	// no I need to check if it is a expression and if so redo the scanning...
	input_tokens = expr_token_list;
	expr_token_list = NULL;
	tmp = input_tokens;
	while (tmp)
	{
			// debug("EXPRESSION");
			token = (t_token *)tmp->content;
			if (token->type == EXPRESSION)
			{
				debug("EXPRESSION");
				// remove parenthesis from content
				char *lexem = ft_substr(token->lexeme, 1, ft_strlen(token->lexeme) - 2);
				debug("lexem %s", lexem);
				token->type = COMMAND;
				// create a new token list
				data->input = lexem;
				t_list *new_token_list = tokenizer(data);
				if (new_token_list == NULL)
					return (NULL);
				// free the old token list
				// ft_lstclear(&expr_token_list, free);	
				return (create_ast(data, new_token_list));					
			}
		// as long as I dont find an expression I will add the token to the list
		ft_lstadd_back(&expr_token_list, ft_lstnew(token));
		tmp = tmp->next;	
	}
	// if I get here I am a terminal node 
	node = new_node(NODE_TERMINAL, NULL, NULL, expr_token_list);
	return (node);
}


void		free_ast(t_ast_node *ast)
{
	if (ast==NULL)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	ft_lstclear(&ast->token_list, free);
	free(ast);
}

/* 
need to print the ast tree  
and each node should be printed with its type to debug
like NODE_EXPRESSION, which is the default node
..need to think if I want to assign a type to each node here
or in the analyser?
NODE_COMMAND, NODE_PIPE, NODE_REDIRECT, NODE_HEREDOC
also, the args should be printed

still working on it - laurent
the content of each node in the linked list is a t_token

struct s_token {
	t_tokentype	type;
	char		*lexeme;
	int 		start;
};
typedef struct s_token t_token;
*/
void print_ast(t_ast_node *ast)
{
	if (ast == NULL)
		return ;
	// debug("\nparser");


    // print node
	t_list *tokenlist;
	t_token *token;

	tokenlist = ast->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return ;
	// traverse the token list
	while (tokenlist)
	{
		token = (t_token *)tokenlist->content;
		debug("token type: %d", (t_tokentype)(token->type));
		debug("lexeme %s", (char *)(token->lexeme));
		tokenlist = tokenlist->next;
	}

	// analyze node
	// expand any lexemes that need to be expanded

	// go to the next nodes and repeat
	debug("---------left -----------");
	if (ast->left)
		print_ast(ast->left);
	else
		debug("left is NULL");
	debug("---------right -----------");
	if (ast->right)
		print_ast(ast->right);
	else
		debug("right is NULL");
}