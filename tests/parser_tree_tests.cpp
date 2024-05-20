#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/scanner.h"
#include "../include/parser.h"

t_data *g_data;

/*
testing for     
> This operator redirects the output of a command to a file. 
If the file already exists, it is overwritten. 
If it doesn't exist, it is created. 
For example, echo Hello > file.txt writes "Hello" to file.txt, 
overwriting its contents if it already exists.

>| 
This is similar to > ,
but it will overwrite the file even if the noclobber 
option has been set in the shell. 
The noclobber option prevents overwriting of files by the > operator.

>& 
This operator redirects one file descriptor to another. 
For example, command 2>&1 redirects the standard error (file descriptor 2) to standard output (file descriptor 1).

&> 
This operator redirects both standard output and standard error 
to a file. It's equivalent to command > file 2>&1. 
For example, command &> file redirects both the standard output and standard error of command to file.

>> This operator appends the output of a command to a file. 
If the file doesn't exist, it is created. 
For example, echo Hello >> file.txt appends "Hello" to file.txt, 
preserving its existing contents.
*/
/*
I am still looking for a way to make this function common to all tests files
but it is complicated by the fact that it is a mix of C and C++ code
so for now i will keep it in every file that needs it
*/
const char* process_token(t_list **current, int *i, const char* expected_lexeme, t_tokentype expected_tokentype) 
{
	// first check if at the end of the list to avoid segfault
	if (*current == NULL && expected_lexeme == NULL && expected_tokentype == NULL_TOKEN) 
		return NULL;
	
    t_token *token = (t_token *)(*current)->content;
	
    debug("\ntoken number %d ", *i);
    debug("token type: %d ", token->type);
    debug("token lexeme: -%s- \n", token->lexeme);
    my_assert(token->type == expected_tokentype, "token type is not the expected type..." );
    my_assert(strcmp(token->lexeme, expected_lexeme) == 0, "token lexeme is not expected");
    *current = (*current)->next;
    (*i)++;
	return NULL;
}

const char* process_ast_node(t_ast_node *ast, t_nodetype expected_node_type, t_tokentype expected_type, const char *expected_lexeme) 
{
	t_token *token = (t_token *)ast->token_list->content;
	t_tokentype token_type = token->type;
	t_nodetype node_type = ast->type;
	debug("ast node type: %d ", node_type);
	debug("ast node token type: %d ", token_type);
	debug("ast node token lexeme: %s ", token->lexeme);
	if (ast == NULL)
		return "ast is null";
	else if (node_type != expected_node_type)
		return "ast type is not expected type";
	else if (ast->token_list == NULL)
		return "astnode token list is null";
	else if (ast->token_list->content == NULL)
		return "astnode token list content is null";
	else if (token_type != expected_type)
		return "token type is not expected type";
	else if (token->lexeme == NULL)
		return "token lexeme is null";
	else if (strcmp(token->lexeme, expected_lexeme) != 0)
		return "token lexeme is not expected lexeme";
	return NULL;
}


/*
This is a test function. It should return NULL if the test passes
debug will print to the log
log_err will print to the error log
my_assert will check a condition and return an error message if it fails
run_test will run a test function and return the error message if it fails
RUN_TESTS will run all the tests and print the results

in this test I will just have one node in the tree
*/
const char* test_parser_tree_simple_command() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "/bin/ls -la";
	const char* input = str.c_str();
	init_data(&g_data);
	g_data->input = input;
	t_list *lexemes = tokenizer(g_data->input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;

	result = process_token(&current, &i, "/bin/ls", PATHNAME);
	result = process_token(&current, &i, "-la", FLAGS);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	t_ast_node *ast = create_ast(lexemes);
	t_token *token = (t_token *)ast->token_list->content;
	t_tokentype token_type = token->type;
	debug("ast node type: %d ", ast->type);
	debug("ast node token type: %d ", token_type);
	debug("ast node token lexeme: %s ", token->lexeme);

	result = process_ast_node(ast, NODE_TERMINAL, PATHNAME, "/bin/ls");
	my_assert(ast->left == NULL, "left node not null");
	my_assert(ast->right == NULL, "right node not null");

	return result;
}

/*
in this test I will just have one node in the tree
*/
const char* test_parser_tree_simple_command2() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "echo \"hello world\"";
	const char* input = str.c_str();
	init_data(&g_data);
	g_data->input = input;
	t_list *lexemes = tokenizer(g_data->input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;

	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "\"hello world\"", QUOTED_STRING);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	t_ast_node *ast = create_ast(lexemes);
	t_token *token = (t_token *)ast->token_list->content;
	t_tokentype token_type = token->type;
	debug("ast node type: %d ", ast->type);
	debug("ast node token type: %d ", token_type);
	debug("ast node token lexeme: %s ", token->lexeme);

	result = process_ast_node(ast, NODE_TERMINAL, BUILTIN, "echo");
	my_assert(ast->left == NULL, "left node not null");
	my_assert(ast->right == NULL, "right node not null");

	return result;
}

/*
with parentheses

DEBUG src/parser.c:467: token type: 5
DEBUG src/parser.c:468: lexeme |
DEBUG src/parser.c:467: token type: 21
DEBUG src/parser.c:468: lexeme false
DEBUG src/parser.c:467: token type: 9
DEBUG src/parser.c:468: lexeme ||
DEBUG src/parser.c:467: token type: 20
DEBUG src/parser.c:468: lexeme true
DEBUG src/parser.c:467: token type: 0
DEBUG src/parser.c:468: lexeme sdf
*/
const char *test_parser_tree_simple_command3() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "false | (true || sdf)";
	const char* input = str.c_str();
	init_data(&g_data);
	g_data->input = input;
	t_list *lexemes = tokenizer(g_data->input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;	

	result = process_token(&current, &i, "false", FALSETOK);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "(true || sdf)", EXPRESSION);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	t_ast_node *ast = create_ast(lexemes);

	t_token *token = (t_token *)ast->token_list->content;
	t_tokentype token_type = token->type;
	debug("ast node type: %d ", ast->type);
	debug("ast node token type: %d ", token_type);
	debug("ast node token lexeme: %s ", token->lexeme);

	// print all the nodes from the ast
	// print_ast(ast);
	
	//here I need to walk the tree and check the nodes
	
	
	
	result = process_ast_node(ast, NODE_PIPELINE, PIPE, "|");
	result = process_ast_node(ast->left, NODE_TERMINAL, FALSETOK, "false");

	result = process_ast_node(ast->right, NODE_LIST, OR_IF, "||");
	result = process_ast_node(ast->right->left, NODE_TERMINAL, TRUETOK, "true");
	result = process_ast_node(ast->right->right, NODE_TERMINAL, WORD, "sdf");
	

	return result;
}

/*
*/
const char *test_parser_tree_simple_command4() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "false | (true || mhh) ha!";
	const char* input = str.c_str();
	init_data(&g_data);
	g_data->input = input;
	t_list *lexemes = tokenizer(g_data->input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;	

	result = process_token(&current, &i, "false", FALSETOK);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "(true || mhh)", EXPRESSION);
	result = process_token(&current, &i, "ha!", WORD);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	t_ast_node *ast = create_ast(lexemes);

	t_token *token = (t_token *)ast->token_list->content;
	t_tokentype token_type = token->type;
	debug("ast node type: %d ", ast->type);
	debug("ast node token type: %d ", token_type);
	debug("ast node token lexeme: %s ", token->lexeme);

	// print all the nodes from the ast
	// print_ast(ast);
	
	//here I need to walk the tree and check the nodes
	
	result = process_ast_node(ast, NODE_PIPELINE, PIPE, "|");
	result = process_ast_node(ast->left, NODE_TERMINAL, FALSETOK, "false");
	result = process_ast_node(ast->right, NODE_LIST, OR_IF, "||");
	result = process_ast_node(ast->right->left, NODE_TERMINAL, TRUETOK, "true");
	result = process_ast_node(ast->right->right, NODE_TERMINAL, WORD, "mhh");	
	

	return result;
}

const char *test_parser_tree_simple_command5() {
	std::string str = "false || true && false | true && false";
	const char* input = str.c_str();
	init_data(&g_data);
	g_data->input = input;
	t_list *lexemes = tokenizer(g_data->input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;	

	result = process_token(&current, &i, "false", FALSETOK);
	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, "&&", AND_IF);
	result = process_token(&current, &i, "false", FALSETOK);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, "&&", AND_IF);
	result = process_token(&current, &i, "false", FALSETOK);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	t_ast_node *ast = create_ast(lexemes);

	t_token *token = (t_token *)ast->token_list->content;
	t_tokentype token_type = token->type;
	debug("ast node type: %d ", ast->type);
	debug("ast node token type: %d ", token_type);
	debug("ast node token lexeme: %s ", token->lexeme);


	// print all the nodes from the ast
	// print_ast(ast);
	//false || true && false | true && false
	//here I need to walk the tree and check the nodes
	result = process_ast_node(ast, NODE_LIST, AND_IF, "&&");
	result = process_ast_node(ast->left, NODE_LIST, AND_IF, "&&");
	result = process_ast_node(ast->left->left, NODE_LIST, OR_IF, "||");
	result = process_ast_node(ast->left->left->left, NODE_TERMINAL, FALSETOK, "false");
	result = process_ast_node(ast->left->left->right, NODE_TERMINAL, TRUETOK, "true");
	result = process_ast_node(ast->left->right, NODE_PIPELINE, PIPE, "|");
	result = process_ast_node(ast->left->right->right, NODE_TERMINAL, TRUETOK, "true");
	result = process_ast_node(ast->left->right->left, NODE_TERMINAL, FALSETOK, "false");
	result = process_ast_node(ast->right, NODE_TERMINAL, FALSETOK, "false");

	return result;
}

/*
DEBUG src/parser.c:498: level 4 - token type: 5 - lexeme |
DEBUG src/parser.c:498: level 5 - token type: 5 - lexeme |
DEBUG src/parser.c:498: level 6 - token type: 1 - lexeme 111
DEBUG src/parser.c:498: level 7 - token type: 1 - lexeme 222
DEBUG src/parser.c:498: level 8 - token type: 1 - lexeme 333
*/
const char *test_parser_tree_simple_command6() {
	std::string str = "111|222    | 333";
	const char* input = str.c_str();
	init_data(&g_data);
	g_data->input = input;
	t_list *lexemes = tokenizer(g_data->input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;	

	result = process_token(&current, &i, "111", NUMBER);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "222", NUMBER);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "333", NUMBER);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	t_ast_node *ast = create_ast(lexemes);

	t_token *token = (t_token *)ast->token_list->content;
	t_tokentype token_type = token->type;
	debug("ast node type: %d ", ast->type);
	debug("ast node token type: %d ", token_type);
	debug("ast node token lexeme: %s ", token->lexeme);


	// print all the nodes from the ast
	// print_ast(ast);

	//here I need to walk the tree and check the nodes
	result = process_ast_node(ast, NODE_PIPELINE, PIPE, "|");
	result = process_ast_node(ast->left, NODE_PIPELINE, PIPE, "|");
	result = process_ast_node(ast->left->left, NODE_TERMINAL, NUMBER, "111");
	result = process_ast_node(ast->left->right, NODE_TERMINAL, NUMBER, "222");
	result = process_ast_node(ast->right, NODE_TERMINAL, NUMBER, "333");

	return result;
}

const char *test_parser_tree_simple_command7() {
	std::string str = "()";
	const char* input = str.c_str();
	init_data(&g_data);
	g_data->input = input;
	t_list *lexemes = tokenizer(g_data->input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;	

	result = process_token(&current, &i, "()", EXPRESSION);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	t_ast_node *ast = create_ast(lexemes);
	assert(ast == NULL);

	return result;
}

//a | (b)
const char *test_parser_tree_simple_command8() {
	std::string str = "a | (b)";
	const char* input = str.c_str();
	init_data(&g_data);
	g_data->input = input;
	t_list *lexemes = tokenizer(g_data->input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;	

	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "(b)", EXPRESSION);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	t_ast_node *ast = create_ast(lexemes);
	
	t_token *token = (t_token *)ast->token_list->content;
	t_tokentype token_type = token->type;
	debug("ast node type: %d ", ast->type);
	debug("ast node token type: %d ", token_type);
	debug("ast node token lexeme: %s ", token->lexeme);

	// print all the nodes from the ast
	// print_ast(ast);

	//here I need to walk the tree and check the nodes
	result = process_ast_node(ast, NODE_PIPELINE, PIPE, "|");
	result = process_ast_node(ast->left, NODE_TERMINAL, WORD, "a");
	result = process_ast_node(ast->right, NODE_TERMINAL, WORD, "b");

	return result;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_parser_tree_simple_command);	
	run_test(test_parser_tree_simple_command2);	
	run_test(test_parser_tree_simple_command3);
	run_test(test_parser_tree_simple_command4);
	// run_test(test_parser_tree_simple_command5);
	// run_test(test_parser_tree_simple_command6);
	// run_test(test_parser_tree_simple_command7);
	run_test(test_parser_tree_simple_command8);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
