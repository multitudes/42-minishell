/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:39:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/23 11:59:26 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
grammar:
to get the final table
list      -> pipeline (";" | "&" | "&&" | "||") pipeline)* [";"] | ["&"] ["\n"]
			| "(" list ")";
pipeline ->  command  (("|" | "|&" | ";" | "&&" | "||" )command)* ;
			| "(" list ")";
command ->  	  simple_command 
				| builtin 
				| DLESS 
				| redirection
				| [time [-p]] [!] expression
				| "(" list ")";

simple_command 	-> name (args)* ;
builtin 		-> name (args)* ; 
redirection 	-> expression ( "<" | ">" | ">>" | ">>&" | "2>" | "&> | &>> | 2>> | <> | >|") expression; 
DLESS 			-> expression "<<" delimiter newline content delimiter;

delimiter -> STRING;
content -> MULTIPLE_LINE_TEXT;
flags 	-> FLAGS;
name 	-> WORD | COM_EXPANSION | VAR_EXPANSION;
args 	-> FLAGS | WORD | STRING | QUOTED_STRING | SIMPLE_QUOTED_STRING | VAR_EXPANSION | EXPR_EXPANSION;

The operators "&&" and "||" shall have equal precedence and shall be evaluated with left associativity. For example, both of the following commands write solely bar to standard output:

false && echo foo || echo bar // false && echo foo is false so echo bar is executed
true || echo foo && echo bar // true || echo foo is true so echo bar is executed


For this subject:
<: Redirects standard input from a file.
>: Redirects standard output to a file, overwriting the file if it exists.
>>: Redirects standard output to a file, appending to the file if it exists.
<<: Here Document

There are a few more redirection operators that you might encounter: (copilot says)
>>&: Redirects both standard output and standard error to a file, appending 
to the file if it exists.
2>: Redirects standard error to a file, overwriting the file if it exists.
&>: Redirects both standard output and standard error to a file, overwriting
 the file if it exists.
2>>: Redirects standard error to a file, appending to the file if it exists.
<>: Opens a file for both reading and writing.
>|: Redirects standard output to a file, overwriting the file even if 
the noclobber option has been set in the shell.
&>>: Redirects both standard output and standard error to a file, 
appending to the file if it exists.

we will implement a parser method called "recursive descent,"
which is a top-down parser. 
*/

/*
I sometimes need to return NULL on error
*/
void *return_null_on_err(char *message)
{
	perror(message);
	return (NULL);
}

/*

*/
t_ast_node* new_node(t_nodetype type, t_ast_node* left, t_ast_node* right, t_list *token_list)
{
	t_ast_node *node;

	if (token_list == NULL)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (return_null_on_err("malloc failed in new_node"));
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
	t_token *token;
	if (input_token == NULL)
		return ;
	token = (t_token *)input_token->content;
	if (token == NULL)
		return ;
	printf("token type: %d, %s", token->type, token->lexeme);
}

/*
JUST a check for the type of token
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
		if (token->type == REDIRECT_OUT || token->type == REDIRECT_IN || \
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
	int count;
	t_list *tmp;
	
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
Since each node in the ast has a morcel of the input tokens
linked list I need to break the list at the node
so that they do not interfere with each other in different part
of the tree
*/
void break_list(t_list **input_tokens)
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

/*
This function is used to consume a token from the input_tokens list but also
needs to break the list before the node 
*/
t_token *consume_token(t_list **input_tokens)
{
	t_token *token;
	
	if (*input_tokens == NULL)
		return (NULL);
	token = (t_token *)(*input_tokens)->content;
	*input_tokens = (*input_tokens)->next;
	break_list(input_tokens);
	if (!input_tokens || !*input_tokens)
		return (NULL);
	return (token);
}

/*
safely get the current token
*/
t_token	*get_curr_token(t_list *input_tokens)
{
	t_token *token;
	
	if (input_tokens == NULL)
		return (NULL);
	token = (t_token *)input_tokens->content;
	return (token);
}

/*
safely get the lexeme of the current token
*/
char *get_token_lexeme(t_list *input_tokens)
{
	t_token *token;
	
	if (input_tokens == NULL)
		return (NULL);
	token = (t_token *)input_tokens->content;
	return (token->lexeme);
}

/*
safely get the type of the current token
*/
t_tokentype get_token_type(t_list *input_tokens)
{
	t_token *token;
	
	if (input_tokens == NULL)
		return (0);
	token = (t_token *)input_tokens->content;
	return (token->type);
}

bool	token_list_has_astnode(t_list *new_token_list)
{
	t_list *tmp;
	
	tmp = new_token_list;
	if (new_token_list == NULL)
		return (false);		
	while (tmp)
	{
		if (is_not_control_token(get_curr_token(tmp)))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

/*
I now take care of empty expressions in the scanner
NOTE: I do not change head if there is a node before the expression
new_token_list from the tokenizer is the new list of tokens
and if that is null I already have the scanner error message 
on stderr and do not need to print anothererror... I will continue?
return -1?
*/
bool replace_expression_tokens(t_list **head, t_list **input_tokens)
{
	t_list	*expr_node;
	t_token	*curr_token;
	t_list	*new_token_list;
	bool	has_node;

	has_node = false;
	expr_node = *input_tokens;
	curr_token = get_curr_token(expr_node);
	new_token_list = tokenizer(ft_substr(curr_token->lexeme, 1, ft_strlen(curr_token->lexeme) - 2));
	// check if I have a token delimiter in my new token list
	if (new_token_list == NULL)
		return (false);
	has_node = token_list_has_astnode(new_token_list);
	*input_tokens = (*input_tokens)->next;
	if (*input_tokens)
	{
		(*input_tokens)->prev = ft_lstlast(new_token_list);
		ft_lstlast(new_token_list)->next = *input_tokens;
	}
	if ((expr_node)->prev)
	{
		expr_node->prev->next = new_token_list;
		new_token_list->prev = expr_node->prev;
	}
	else
		*head = new_token_list;
	free(curr_token->lexeme);
	free(curr_token);
	free(expr_node);
	return (has_node);
}

/*
I get a t_list node in input with my token as expression type
and want to substitute it with the content of the expression
it is like cut and paste a linked list
if I have an expression and cant replace it then I keep on running?
*/
bool	extract_expression(t_list **head, t_list **input_tokens)
{
	if (get_token_type(*input_tokens) == EXPRESSION)
		return (replace_expression_tokens(head, input_tokens));
	return (false);
}

bool	is_not_control_token(t_token *token)
{
	if (token == NULL)
		return (false);
	if (token->type == PIPE || token->type == PIPE_AND || token->type == AND_IF || \
	token->type == OR_IF || token->type == SEMI || token->type == AND_TOK)
		return (false);
	return (true);
}

/*
follows the grammar
*/
t_ast_node *parse_terminal(t_list **input_tokens)
{
	t_list	*head;
	t_ast_node	*a;
	bool	expr_has_node;

	a = NULL;
	expr_has_node = false;
	head = *input_tokens;
	if (input_tokens == NULL || *input_tokens == NULL)
		return (NULL);
	// print_token(head->prev);
	while (is_not_control_token(get_curr_token(*input_tokens)))
	{
		if (extract_expression(&head, input_tokens))
			expr_has_node = true;
		if (head == NULL)
			return (NULL);
		if (input_tokens == NULL || *input_tokens == NULL)
			break;
		if (is_not_control_token(get_curr_token(*input_tokens)))
			*input_tokens = (*input_tokens)->next;
	}
	break_list(input_tokens);
	
	// if I had an expression I need to return a node with the expression content
	// so I need to reparse the list
	debug("had expression: %d", expr_has_node);
	if (expr_has_node)
	{
		t_list *tmp = head;
		t_token *token2;
		token2 = (t_token *)tmp->content;
		debug("HEAD pointer %p", head);
		// debug("HEAD token type: %d, %s", token2->type, token2->lexeme);
		while (tmp)
		{
			token2 = (t_token *)tmp->content;
			debug("token type: %d, %s", token2->type, token2->lexeme);
			tmp = tmp->next;
			if (tmp)
				debug("next token type: %p %p", tmp, ((t_token *)(tmp->content)));
		}
		debug("parse as list because expression");
		a = parse_list(&head);
		if (a)
			debug("new ast node from expression in parse_terminal: %d", a->type);
		return (a);
	}
	else
		a = new_node(NODE_TERMINAL, NULL, NULL, head);
	if (head->prev)
		head->prev = NULL;
	if (head)
		debug("new terminal node with head lexeme: %s", ((t_token *)(head->content))->lexeme);
	if (a)
		debug("new ast node type in parse_terminal: %d", a->type);
	return (a);
}

bool is_pipe_token(t_list *input_tokens)
{
	t_token *token;
	
	if (input_tokens == NULL)
		return (false);
	token = (t_token *)input_tokens->content;
	if (token->type == PIPE || token->type == PIPE_AND)
		return (true);
	return (false);
}

t_ast_node	*parse_pipeline(t_list **input_tokens)
{
	t_ast_node *a;
	t_token *token;

	a = NULL;
	token = NULL;
	if (input_tokens == NULL || *input_tokens == NULL)
		return (NULL);
	a = parse_terminal(input_tokens);
	if (a == NULL)
		return (NULL);
	while (is_pipe_token(*input_tokens))
	{
			token = get_curr_token(*input_tokens);
			if (consume_token(input_tokens) == NULL)
				return (NULL);
			t_ast_node *b = parse_terminal(input_tokens);
			if (b == NULL)
				return (free_ast(&a));
			a = new_node(NODE_PIPELINE, a, b, ft_lstnew(token));
			if (a == NULL)
				return (free_ast(&a));
	}
	return (a);
}
	
/**/
t_ast_node	*parse_list(t_list **input_tokens)
{
	t_token *token;
	t_ast_node *a;
	t_ast_node *b;

	a = NULL;
	b = NULL;
	token = NULL;
	if (input_tokens == NULL || *input_tokens == NULL)
		return (NULL);
	a = parse_pipeline(input_tokens);
	if (a == NULL)
		return (NULL);
	while (*input_tokens)
	{
		token = get_curr_token(*input_tokens);
		if (token->type == AND_IF || token->type == OR_IF || token->type == EXPRESSION)
		{
			if (consume_token(input_tokens) == NULL)
				return (NULL);
			b = parse_pipeline(input_tokens);
			if (b == NULL)
				return (free_ast(&a));
			a = new_node(NODE_LIST, a, b, ft_lstnew(token));
			if (a == NULL)
				return (free_ast(&a));
		}
		if (*input_tokens)
			debug("in parse list - extraneus token type: %d, %s", ((t_token *)(*input_tokens)->content)->type, ((t_token *)(*input_tokens)->content)->lexeme);
	}
	return (a);
}

/*
introducing a new node type for the tree, t_ast_node.
The tree will be composed of nodes, each node will have a type,
a left and a right node, and a list of tokens as a t_list.
*/
t_ast_node *create_ast(t_list *input_tokens)
{
	t_ast_node *a;
	t_list *tmp;
	
	a = NULL;
	tmp = input_tokens;
	if (input_tokens == NULL)
		return (NULL);		
	while (tmp)
	{
		a = parse_list(&tmp);
		if (!a)
			return (NULL);
		print_ast(a);
	}
	return (a);
}


void		*free_ast(t_ast_node **ast)
{
	if (ast == NULL)
		return (NULL);
	free_ast(&((*ast)->left));
	free_ast(&((*ast)->right));
	ft_lstclear(&((*ast)->token_list), free);
	free(*ast);
	*ast = NULL;
	return (NULL);
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
void print_ast(t_ast_node *a)
{
	static int level = 0;
	t_list *tokenlist;
	t_token *token;

	if (a == NULL)
	{
		debug("ast is NULL");
		return ;
	}
	tokenlist = a->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return ;
	// traverse the token list
	while (tokenlist)
	{
		token = (t_token *)tokenlist->content;
		debug("level %d - token type: %d - lexeme %s", level, (t_tokentype)(token->type), token->lexeme);
		tokenlist = tokenlist->next;
	}

	// analyze node
	// expand any lexemes that need to be expanded

	// go to the next nodes and repeat
	// debug("---------left -----------");
	if (a->left)
	{		
		level++;
		print_ast(a->left);
	}
	//debug("---------right -----------");
	if (a->right)
	{
		level++;
		print_ast(a->right);
	}
}