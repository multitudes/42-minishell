/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:39:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/19 17:24:30 by lbrusa           ###   ########.fr       */
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

void print_token(void *token)
{
    t_token *t;
    t = (t_token *)((t_list *)token)->content;
    debug("token type: %d, %s", t->type, t->lexeme);
}

// I get a t_list node in inputwith my token as expression type
// andwant to substitute it with the content of the expression
bool	extract_expression(t_list **head, t_list **input_tokens)
{
	t_list	*tmp;
	t_token	*curr_token; 
	t_list	*next_old_list;

	curr_token = (t_token *)(*input_tokens)->content;
	if (input_tokens && *input_tokens && curr_token->type == EXPRESSION)
	{
		debug("EXPRESSION");
		next_old_list = (*input_tokens)->next;

		// will remove the current node but first saving it to a tmp to free it later
		tmp = *input_tokens;

		// remove parenthesis from content
		char *lexem = ft_substr(curr_token->lexeme, 1, ft_strlen(curr_token->lexeme) - 2);
		debug("lexem %s", lexem);

		// create a new token list
		t_list *new_token_list = tokenizer(lexem);
		// debug("new_token_list %s", ((t_token *)(new_token_list->content))->lexeme);

		if (new_token_list)
		{
			t_list *newlist_last = ft_lstlast(new_token_list);
			// check the previous it was not the first node in the list
			// check the next connection - not the last node in the list
			if (next_old_list)
			{
				debug("not last node in the list");
				next_old_list->prev = newlist_last;
				newlist_last->next = next_old_list;
			}
			// last node in the list
			else if (next_old_list == NULL)
			{
				debug("last node in the list");
			}
			if ((*input_tokens)->prev)
			{
				debug("not first node in the list");
				debug("prev %s", ((t_token *)(*input_tokens)->prev->content)->lexeme);
				(*input_tokens)->prev->next = new_token_list;
				new_token_list->prev = (*input_tokens)->prev;
			}
			else
			{
				*head = new_token_list;
			}
			*input_tokens = next_old_list;
		}
		// free the old token list
		// free(lexem);
		free(curr_token->lexeme);
		ft_lstdelone(tmp, free);
		tmp = NULL;
		// ft_lstiter(*input_tokens, print_token);
		// debug("IN EXPRESSION return true");
		return (true);
	}	
	return (false);
}

/*
follows the grammar
*/
t_ast_node *parse_terminal(t_list **input_tokens)
{
	bool has_expr;
	t_ast_node *a;
	t_list *head;
	t_token *token;

	a = NULL;
	has_expr = false;
	head = *input_tokens;
	if (*input_tokens == NULL || input_tokens == NULL)
		return (NULL);
	token = (t_token *)(*input_tokens)->content;
	
	// debug the prev and next pointer of current token
	// if ((t_token *)(*input_tokens)->prev)
	// 	debug("\nprev %s", ((t_token *)(*input_tokens)->prev->content)->lexeme);
	// if ((t_token *)(*input_tokens)->next)
	// 	debug("next %s", ((t_token *)(*input_tokens)->next->content)->lexeme);

	while (*input_tokens && (token->type != PIPE) && (token->type != PIPE_AND) && (token->type != AND_IF) && (token->type != OR_IF))
	{
		token = (t_token *)(*input_tokens)->content;
		debug("parse_terminal token type: %d, %s", token->type, token->lexeme);
		// debug the prev and next pointer of current token
		// if ((t_token *)(*input_tokens)->prev)
		// 	debug("\nprev %s", ((t_token *)(*input_tokens)->prev->content)->lexeme);
		// if ((t_token *)(*input_tokens)->next)
		// 	debug("next %s", ((t_token *)(*input_tokens)->next->content)->lexeme);

		// write a function to handle the type expression
		if (extract_expression(&head ,input_tokens))
		{
			has_expr = true;
			// DEBUG : print every token in the list
			if (head == NULL)
				debug("head is NULL");
			else
				debug("head is not NULL");
			t_list *tmp = head;
			t_token *token2;
			token2 = (t_token *)tmp->content;
			debug("HEAD pointer %p", head);
			debug("HEAD token type: %d, %s", token2->type, token2->lexeme);
			while (tmp)
			{
				token2 = (t_token *)tmp->content;
				debug("token type: %d, %s", token2->type, token2->lexeme);
				tmp = tmp->next;
			}
			if (*input_tokens)
			{
				token = (t_token *)(*input_tokens)->content;	
				debug("AFTER EXPRESSION: token type: %d, %s", token->type, token->lexeme);
			}
			continue;
		}
		if (has_expr)
		{
			debug("why am I here????");
			// continue;
		}
		if (*input_tokens == NULL)
			continue;
		*input_tokens = (*input_tokens)->next;
		if (*input_tokens)
			token = (t_token *)(*input_tokens)->content;
		// if (token->type == PIPE || token->type == PIPE_AND || token->type == AND_IF || token->type == OR_IF || token->type == SEMI || token->type == AND_TOK)
		// 	break;
	}
	// debug("new node - head content: %s", ((t_token *)(head->content))->lexeme);
	
	// if *input_tokens is not null I have a delimiter like a pipe or pipe_and
	// debug the prev and next pointer of current token
	if (*input_tokens)
	{	
		token = (t_token *)(*input_tokens)->content;
		debug("AFTER LOOP EXIT: token type: %d, %s", token->type, token->lexeme);
	}
	// if I have a pipe or pipe_and I need to break the list before my node and
	// pass head to create a node with the previous list until the previous token
	// ex || is not in the terminal node
	if (*input_tokens)
	{
		if ((*input_tokens)->prev)
		{	
			// debug("prev %s", ((t_token *)(*input_tokens)->prev->content)->lexeme);
			(*input_tokens)->prev->next = NULL;
			(*input_tokens)->prev = NULL;
			// (*input_tokens)->prev->next = NULL;
		}
	}
	// if I had an expression I need to add a node with the expression content
	if (has_expr)
	{ 		t_list *tmp = head;
			t_token *token2;
			token2 = (t_token *)tmp->content;
			debug("HEAD pointer %p", head);
			debug("HEAD token type: %d, %s", token2->type, token2->lexeme);
			while (tmp)
			{
				token2 = (t_token *)tmp->content;
				debug("token type: %d, %s", token2->type, token2->lexeme);
				tmp = tmp->next;
			}	
		a = parse_list(&head);
		if (a)
			debug("new ast node from extression in parse_terminal: %d", a->type);
	}
	else
		a = new_node(NODE_TERMINAL, NULL, NULL, head);
	if (a)
		debug("new ast node type in parse_terminal: %d", a->type);
	return (a);
}

t_ast_node	*parse_pipeline(t_list **input_tokens)
{
	t_ast_node *a;
	t_token *token;

	a = NULL;
	token = NULL;
	if (*input_tokens == NULL || input_tokens == NULL)
		debug("input tokens is NULL");
		//return (NULL);
	debug("parse_pipeline");

	a = parse_terminal(input_tokens);
	if (a && *input_tokens)
	{	
		debug("got asttype in parse_pipeline: %d and content lexem %s", a->type, ((t_token *)(a->token_list->content))->lexeme);
		debug("in parse pipeline - existing token type: %d, %s", ((t_token *)(*input_tokens)->content)->type, ((t_token *)(*input_tokens)->content)->lexeme);
		token = (t_token *)(*input_tokens)->content;
	}
	// // debug the prev and next pointer of current token
	// if ((t_token *)(*input_tokens)->prev)
	// 	debug("\nprev %s", ((t_token *)(*input_tokens)->prev->content)->lexeme);
	// if ((t_token *)(*input_tokens)->next)
	// 	debug("next %s", ((t_token *)(*input_tokens)->next->content)->lexeme);

	while (*input_tokens && (((t_token *)(*input_tokens)->content)->type == PIPE || ((t_token *)(*input_tokens)->content)->type == PIPE_AND))
	{
		debug("PIPE");
		*input_tokens = (*input_tokens)->next; //consume the pipe or pipe_and token
		if (!*input_tokens)		
		{
			// write(2, "syntax error near unexpected token `newline'\n", 44);
			debug("syntax error near unexpected token `newline'");
			free_ast(a);
			return (NULL);
		}
		(*input_tokens)->prev = NULL;
		t_ast_node *b = parse_terminal(input_tokens);
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
	while (*input_tokens)
	{
		// check for && and ||
		token = (t_token *)(*input_tokens)->content;
		debug("in parse list - new token type: %d, %s", ((t_token *)(*input_tokens)->content)->type, ((t_token *)(*input_tokens)->content)->lexeme);
		if (token->type == AND_IF || token->type == OR_IF)
		{
			debug("check for && and ||");
			*input_tokens = (*input_tokens)->next; //consume the && or || token
			if (!input_tokens || !*input_tokens)
			{
				// write(2, "syntax error near unexpected token `newline'\n", 44);
				debug("syntax error near unexpected token `newline'");
				free_ast(a);
				return (NULL);
			}
			b = parse_pipeline(input_tokens);
			a = new_node(NODE_LIST, a, b, ft_lstnew(token));
		}
		// debug("new ast node type in parse_list: %d", a->type);	
		// if (*input_tokens)
		// 	debug("in parse list - extraneus token type: %d, %s", ((t_token *)(*input_tokens)->content)->type, ((t_token *)(*input_tokens)->content)->lexeme);
		if (*input_tokens == NULL)
			break;
		// *input_tokens = (*input_tokens)->next;
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
		if (a)
			debug("my ast root type in : %d and content lexem %s", a->type, ((t_token *)(a->token_list->content))->lexeme);
		else
			debug("a is NULL");
		debug("\nprint ast - a");
		print_ast(a);
	}
	return (a);
}


void		free_ast(t_ast_node *ast)
{
	if (ast == NULL)
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