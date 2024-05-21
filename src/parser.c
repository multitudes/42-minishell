/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:39:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/21 13:45:52 by lbrusa           ###   ########.fr       */
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
	if (left != NULL)
		left->parent = node;
	if (right != NULL)
		right->parent = node;
	return (node);
}

void	print_token(t_list *input_token)
{
	t_token *token;
	if (input_token == NULL)
	{
		// debug("token is NULL");
		return ;
	}
	token = (t_token *)input_token->content;
	debug("token type: %d, %s", token->type, token->lexeme);
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
	{
		// write(2, "syntax error near unexpected token `newline'\n", 44);
		debug("syntax error near unexpected token `newline'");
		return (NULL);
	}
	return (token);
}

t_token	*get_curr_token(t_list *input_tokens)
{
	t_token *token;
	
	if (input_tokens == NULL)
		return (NULL);
	token = (t_token *)input_tokens->content;
	return (token);
}

char *get_token_lexeme(t_list *input_tokens)
{
	t_token *token;
	
	if (input_tokens == NULL)
		return (NULL);
	token = (t_token *)input_tokens->content;
	return (token->lexeme);
}

t_tokentype get_token_type(t_list *input_tokens)
{
	t_token *token;
	
	if (input_tokens == NULL)
		return (0);
	token = (t_token *)input_tokens->content;
	return (token->type);
}

bool delete_empty_expression(t_list **head, t_list **input_tokens)
{
	t_list *tmp;
	t_token *curr_token;
	t_list *next_token_old_list;

	curr_token = get_curr_token(*input_tokens);
	next_token_old_list = (*input_tokens)->next;
	tmp = *input_tokens;
	debug("empty expression");
	if (*input_tokens && (*input_tokens)->prev)
	{
		debug("prev token is %s", ((t_token *)(*input_tokens)->prev->content)->lexeme);
		(*input_tokens)->prev->next = next_token_old_list;
		if (next_token_old_list)
			next_token_old_list->prev = (*input_tokens)->prev;
	}
	else if ((*input_tokens) && ((*input_tokens)->prev == NULL))
	{
		*head = next_token_old_list;
		if (next_token_old_list)
			next_token_old_list->prev = NULL;
	}
	debug("empty expression extracted");
	*input_tokens = next_token_old_list;
	free(curr_token->lexeme);
	free(curr_token);
	free(tmp);
	return (true);
}

bool replace_expression_tokens(t_list **head, t_list **input_tokens)
{

	t_token *curr_token;
	t_list *tmp;
	t_list *next_token_old_list;

	tmp = *input_tokens;
	next_token_old_list = (*input_tokens)->next;
	curr_token = get_curr_token(*input_tokens);
	char *lexem = ft_substr(curr_token->lexeme, 1, ft_strlen(curr_token->lexeme) - 2);
	t_list *new_token_list = tokenizer(lexem);
	if (new_token_list)
	{
		t_list *newlist_last = ft_lstlast(new_token_list);
		if (next_token_old_list)
		{
			next_token_old_list->prev = newlist_last;
			newlist_last->next = next_token_old_list;
		}
		if ((*input_tokens)->prev)
		{
			(*input_tokens)->prev->next = new_token_list;
			new_token_list->prev = (*input_tokens)->prev;
		}
		else
			*head = new_token_list;
	}

	*input_tokens = next_token_old_list;
	if (head)
		debug("currently the head is %s", ((t_token *)(*head)->content)->lexeme);
	debug("expression extracted");
	free(curr_token->lexeme);
	free(curr_token);
	free(tmp);
	return (true);
}

/*
I get a t_list node in input with my token as expression type
and want to substitute it with the content of the expression
it is like cut and paste a linked list
*/
bool	extract_expression(t_list **head, t_list **input_tokens)
{
	debug("in extract expression and current token is %s", ((t_token *)(*input_tokens)->content)->lexeme);
	if (get_token_type(*input_tokens) == EXPRESSION)
	{
		if (ft_strlen(get_token_lexeme(*input_tokens)) == 2)
			return (delete_empty_expression(head, input_tokens));
		else
			return (replace_expression_tokens(head, input_tokens));
	}
	return (false);
}

bool	is_not_control_token(t_list *input_tokens)
{
	t_token *token;
	
	if (input_tokens == NULL)
		return (false);
	token = (t_token *)input_tokens->content;
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
	bool	has_expr;

	a = NULL;
	has_expr = false;
	head = *input_tokens;
	if (input_tokens == NULL || *input_tokens == NULL)
		return (NULL);
	print_token(head->prev);
	while (is_not_control_token(*input_tokens))
	{
		has_expr = extract_expression(&head, input_tokens);
		if (head == NULL)
			return (NULL);
		if (input_tokens == NULL || *input_tokens == NULL)
			break;
		*input_tokens = (*input_tokens)->next;
	}
	break_list(input_tokens);
	// if I had an expression I need to return a node with the expression content
	// so I need to reparse the list
	debug("has expression: %d", has_expr);
	if (has_expr && head)
	{
		// t_list *tmp = head;
		// t_token *token2;
		// token2 = (t_token *)tmp->content;
		// debug("HEAD pointer %p", head);
		// // debug("HEAD token type: %d, %s", token2->type, token2->lexeme);
		// while (tmp)
		// {
		// 	token2 = (t_token *)tmp->content;
		// 	debug("token type: %d, %s", token2->type, token2->lexeme);
		// 	tmp = tmp->next;
		// 	if (tmp)
		// 		debug("next token type: %p %p", tmp, ((t_token *)(tmp->content)));
		// }
		// debug("parse as list because expression");
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
	{
		debug("input tokens is NULL");
		return (NULL);
	}
	debug("parse_pipeline");

	a = parse_terminal(input_tokens);
	if (a)
		debug("new ast node type in parse_pipeline: %d", a->type);
	else 
		return (NULL);
	while (is_pipe_token(*input_tokens))
	{
			token = (t_token *)(*input_tokens)->content;
			debug("PIPE");
			if (consume_token(input_tokens) == NULL)
				return (NULL);
			t_ast_node *b = parse_terminal(input_tokens);
			if (b == NULL)
				return (free_ast(a));
			a = new_node(NODE_PIPELINE, a, b, ft_lstnew(token));
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
	debug("parse list");
	a = parse_pipeline(input_tokens);
	if (a)
		debug("new ast node type in parse_list: %d", a->type);
	else 
		return (NULL);
	while (*input_tokens)
	{
		// check for && and ||
		token = (t_token *)(*input_tokens)->content;
		debug("in parse list - new token type: %d, %s", ((t_token *)(*input_tokens)->content)->type, ((t_token *)(*input_tokens)->content)->lexeme);
		if (token->type == AND_IF || token->type == OR_IF || token->type == EXPRESSION)
		{
			debug("check for && and ||");
			if (consume_token(input_tokens) == NULL)
				return (NULL);
			b = parse_pipeline(input_tokens);
			if (b == NULL)
				return (free_ast(a));
			a = new_node(NODE_LIST, a, b, ft_lstnew(token));
		}
		// debug("new ast node type in parse_list: %d", a->type);	
		if (*input_tokens)
			debug("in parse list - extraneus token type: %d, %s", ((t_token *)(*input_tokens)->content)->type, ((t_token *)(*input_tokens)->content)->lexeme);
	}
	debug("returning new ast node type in parse_list: %d", a->type);
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
	t_token *token;
	
	a = NULL;
	tmp = input_tokens;
	token = NULL;
	if (input_tokens == NULL)
		return (NULL);		
	while (tmp)
	{
		token = (t_token *)tmp->content;
		debug("token type: %d, %s", token->type, token->lexeme);
		a = parse_list(&tmp);
		if (!a)
			return (NULL);
		if (a)
			debug("my ast root type in : %d and content lexem %s", a->type, ((t_token *)(a->token_list->content))->lexeme);
		else
			debug("a is NULL");
		debug("\nprint ast - a");
		print_ast(a);
	}
	return (a);
}


void		*free_ast(t_ast_node *ast)
{
	if (ast == NULL)
		return (NULL);
	free_ast(ast->left);
	free_ast(ast->right);
	ft_lstclear(&ast->token_list, free);
	free(ast);
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
	// else
	// 	debug("left is NULL");
	//debug("---------right -----------");
	if (a->right)
	{
		level++;
		print_ast(a->right);
	}
	// else
	// 	debug("right is NULL");
}