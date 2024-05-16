/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:39:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/16 09:12:40 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*

grammar:
to get the final table
list      -> pipeline ((";" | "&" | "&&" | "||") pipeline)* [";"] | ["&"] ["\n"]
			| "(" list ")";
pipeline ->  expression  (("|" | "|&" | ";" | "&&" | "||" )expression)* ;
			| "(" list ")";
expression ->  	  command 
				| builtin 
				| DLESS 
				| redirection
				| [time [-p]] [!] expression
				| "(" list ")";

command -> name (args)* ;
builtin -> name (args)* ; 
redirection -> expression ( "<" | ">" | ">>" | ">>&" | "2>" | "&> | &>> | 2>> | <> | >|") expression; 
DLESS -> 	expression "<<" delimiter;

delimiter -> STRING;
content -> MULTIPLE_LINE_TEXT;

name -> WORD | COM_EXPANSION | VAR_EXPANSION;
args -> FLAGS | WORD | STRING | QUOTED_STRING | SIMPLE_QUOTED_STRING | VAR_EXPANSION | EXPR_EXPANSION;


ex
(make &)  | wc -l
make &


GitHub Copilot
The redirection operators you've listed are the most commonly used in bash and similar Unix-like shells. Here's a brief description of each:

<: Redirects standard input from a file.
>: Redirects standard output to a file, overwriting the file if it exists.
>>: Redirects standard output to a file, appending to the file if it exists.
>>&: Redirects both standard output and standard error to a file, appending 
to the file if it exists.
2>: Redirects standard error to a file, overwriting the file if it exists.
&>: Redirects both standard output and standard error to a file, overwriting
 the file if it exists.
There are a few more redirection operators that you might encounter:

2>>: Redirects standard error to a file, appending to the file if it exists.
<>: Opens a file for both reading and writing.
>|: Redirects standard output to a file, overwriting the file even if 
the noclobber option has been set in the shell.
&>>: Redirects both standard output and standard error to a file, 
appending to the file if it exists.


=======================================
to recap :
a list is a pipeline followed by zero or more pipelines separated by a 
semicolon,
 ampersand, double ampersand, or double pipe, or a list in parentheses.
pipeline -> expression (("|" | "|&" | ";")expression* + "&"? ; : 
A pipeline is an expression followed by zero or more expressions 
separated by a pipe (|), pipe and (|&), or semicolon (;). 
The statement can optionally end with an ampersand (&), 
which would run the command in the background.

expression -> command | builtin | DLESS | redirection | "(" statement ")";  
An expression can be a command, a builtin command, a DLESS, a redirection, 
or a statement enclosed in parentheses.


In this file
we will implement a parser method called "recursive descent,"
which is a top-down parser 

So the root is the first pipe or pipe_and operator..

*/
t_ast_node* new_node(t_nodetype type, t_ast_node* left, t_ast_node* right, t_list *token_list)
{
	if (token_list == NULL)
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
	node->token_list = token_list;
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
    DGREAT, // '>>'
so if a node is not an expression I will check if a redirection or DLESS
	DLESS // <<
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
		token->type == DGREAT || token->type == DLESS)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/*
 count the number of nodes in the t_list
*/
int 	count_list(t_list *input_tokens)
{
	t_list *tmp;
	int count;
	
	tmp = input_tokens;
	count = 0;
	if (input_tokens == NULL)
		return (0);		
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

// get the last 

t_ast_node	*parse_terminal(t_list **input_tokens)
{
	t_ast_node *a;
	t_list *head;

	a = NULL;
	head = *input_tokens;
	if (*input_tokens == NULL || input_tokens == NULL)
		return (NULL);
	debug("parse_terminal");
	t_token *token = (t_token *)(*input_tokens)->content;
	while (*input_tokens && token->type != PIPE && token->type != PIPE_AND)
	{
		if (token->type == EXPRESSION)
		{
			debug("EXPRESSION");
			// will remove the current node but first saving it to a tmp
			t_list *tmp = *input_tokens;

			// remove parenthesis from content
			char *lexem = ft_substr(token->lexeme, 1, ft_strlen(token->lexeme) - 2);
			debug("lexem %s", lexem);
			
			// create a new token list
			t_list *new_token_list = tokenizer(lexem);
			debug("new_token_list %s", ((t_token *)(new_token_list->content))->lexeme);

			if (new_token_list)
			{
				// insert the new token list in the current list
				if ((*input_tokens)->prev)
				{
					debug("Not here");
					(*input_tokens)->prev->next = new_token_list;
					new_token_list->prev = (*input_tokens)->prev;
				}
				else
				{
					debug("the current node was the first node");
					new_token_list->prev = NULL;
					head = new_token_list;
				}
				t_list *last = ft_lstlast(new_token_list);
				if ((*input_tokens)->next)
				{
					debug("Not here");
					last->next = (*input_tokens)->next;
					(*input_tokens)->next->prev = last;
				}
				else
				{
					debug("here");
					last->next = NULL;
				}
			}

			*input_tokens = new_token_list;
			// free the old token list
			free(token->lexeme);
			ft_lstdelone(tmp, free);
			free(lexem);
			token = (t_token *)(*input_tokens)->content; // Update token pointer
		}
		debug("token type: %d, %s", token->type, token->lexeme);
		*input_tokens = (*input_tokens)->next;
	}
	// if ((*input_tokens) && (*input_tokens)->prev && (*input_tokens)->next)
	// {
	// 	(*input_tokens)->prev->next = (*input_tokens)->next;
	// 	(*input_tokens)->next->prev = (*input_tokens)->prev;
	// 	debug("prev and next");
	// }
	// else if ((*input_tokens) && (*input_tokens)->prev)
	// {
	// 	(*input_tokens)->prev->next = NULL;
	// }
	// else if ((*input_tokens) && (*input_tokens)->next)
	// {
	// 	(*input_tokens)->next->prev = NULL;
	// }
	// create a new node
	debug("new node - head content: %s", ((t_token *)(head->content))->lexeme);
	a = new_node(NODE_TERMINAL, NULL, NULL, head );
	if (a)
		debug("new type in parse_terminal: %d", a->type);

	return (a);
}


t_ast_node	*parse_pipeline(t_list **input_tokens)
{
	t_ast_node *a;

	a = NULL;
	if (input_tokens == NULL)
		return (NULL);
	a = parse_terminal(input_tokens);
	if (a)
		debug("new type in parse_pipeline: %d and content lexem %s", a->type, ((t_token *)(a->token_list->content))->lexeme);


	return (a);
}

/**/
t_ast_node	*parse_list(t_list **input_tokens)
{
	t_ast_node *a;

	a = NULL;
	if (input_tokens == NULL)
		return (NULL);
	a = parse_pipeline(input_tokens);
	if (a)
		debug("new type in parse_list: %d and content lexem %s", a->type, ((t_token *)(a->token_list->content))->lexeme);


	return (a);
}


/*
introducing a new node type for the tree, t_ast_node.
The tree will be composed of nodes, each node will have a type,
a left and a right node, and a list of tokens as a t_list.
*/
t_ast_node *create_ast(t_data *data, t_list *input_tokens)
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
		debug("token type: %d, %s", token->type, token->lexeme);

		// refactoring to a new parser!! 
		// should return the ast... not yet implemented
		t_ast_node *a = parse_list(&input_tokens);
		if (a)
			debug("new type in : %d and content lexem %s", a->type, ((t_token *)(a->token_list->content))->lexeme);











		// first we  check for && and ||
		if (token->type == AND_IF || token->type == OR_IF)
		{
			debug("AND OR");
			left = create_ast(data, expr_token_list);
			if (left == NULL)
				return (NULL);
			right = create_ast(data, tmp->next);
			if (right == NULL)
				return (NULL);
			node = new_node(NODE_LIST, left, right, ft_lstnew(token));
			// free the token list?
			// ft_lstclear(&expr_token_list, free);
			return (node);
		}
		// as long as I dont find a && or || I will add the token to the list
		ft_lstadd_back(&expr_token_list, ft_lstnew(token));
		tmp = tmp->next;
	}

	// here I need to traverse my list again to check for pipes
	input_tokens = expr_token_list;
	expr_token_list = NULL;
	tmp = input_tokens;
	// che ck for the pipes
	while (tmp)
	{
		token = (t_token *)tmp->content;
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
			node = new_node(NODE_PIPELINE, left, right, ft_lstnew(token));
			// free the token list ? not here because the pointer is used in the tree!
			// ft_lstclear(&expr_token_list, free);
			return (node);
		}
		// as long as I dont find a | or |& I will add the token to the list
		ft_lstadd_back(&expr_token_list, ft_lstnew(token));
		tmp = tmp->next;
	}
	
	// I am here. my token list has no pipe
	// chedck for the redirections if I do not have any pipe
	// here I need to traverse my list again to check for redirections
	input_tokens = expr_token_list;
	expr_token_list = NULL;
	tmp = input_tokens;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		debug("check redir token type: %d, %s", token->type, token->lexeme);

		if (token->type == REDIRECT_OUT || token->type == REDIRECT_BOTH || 
		token->type == GREATAND)
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
			node = new_node(NODE_REDIRECT, left, right, ft_lstnew(token));
			// free the token list?
			// ft_lstclear(&expr_token_list, free);
			return (node);
		}
		// as long as I dont find a << or >>  or < or > I will add the token to the list
		ft_lstadd_back(&expr_token_list, ft_lstnew(token));
		tmp = tmp->next;
	}
	// I am here. my token list has no pipe and no redirection and no DLESSs
	// if I get here it is my terminal the leaf...?
	// no I need to check if it is a expression and if so redo the scanning...
	input_tokens = expr_token_list;
	expr_token_list = NULL;
	tmp = input_tokens;
	while (tmp)
	{
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
			t_list *new_token_list = tokenizer(data->input);
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
	// check if I have a true or false
	token = (t_token *)expr_token_list->content;
	if (count_list(expr_token_list) == 1 && strncicmp(token->lexeme, "true", 4) == 0)
		node = new_node(NODE_TRUE, NULL, NULL, ft_lstnew(token));
	else if (count_list(expr_token_list) == 1 && strncicmp(token->lexeme, "false", 5) == 0)
		node = new_node(NODE_FALSE, NULL, NULL, ft_lstnew(token));
	else
	{
		// if I get here I am a terminal node (command or builtin)
		debug("TERMINAL");
		node = new_node(NODE_TERMINAL, NULL, NULL, expr_token_list);
	}

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
like NODE_LIST, which is the default node
..need to think if I want to assign a type to each node here
or in the analyser?
NODE_COMMAND, NODE_PIPELINE, NODE_REDIRECT, NODE_DLESS
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