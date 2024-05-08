#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/scanner.h"

t_mini_data *g_mini_data;
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
	
    debug("token number %d ", *i);
    debug("token type: %d ", token->type);
    debug("token lexeme: %s ", token->lexeme);
    my_assert(token->type == expected_tokentype, "token type is not WORD");
    my_assert(strcmp(token->lexeme, expected_lexeme) == 0, "token lexeme is not expected");
    *current = (*current)->next;
    (*i)++;
	return NULL;
}

/*
This is a test function. It should return NULL if the test passes
debug will print to the log
log_err will print to the error log
my_assert will check a condition and return an error message if it fails
run_test will run a test function and return the error message if it fails
RUN_TESTS will run all the tests and print the results

I want to check the output of the call to the function in scanner.c file
tokenizer(char *input) returning a t_list of lexemes
I will create a string and check the output of the function
*/
const char* test_scanner_identifiers() {

	std::string str = "hello world";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "hello", WORD);
	result = process_token(&current, &i, "world", WORD);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}


const char* test_scanner_identifiers2() {

	std::string str = "echo he_-3llo world";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "he_-3llo", WORD);	
	result = process_token(&current, &i, "world", WORD);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);	

	return result;
}


const char* test_scanner_identifiers3() {
	std::string str = "||\"\"";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, "\"\"", QUOTED_STRING);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);	
	
	return result;
}

const char* test_scanner_identifiers4() {
	std::string str = "this is a \"quoted\'string\'maybe\"?";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "this", WORD);
	result = process_token(&current, &i, "is", WORD);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "\"quoted\'string\'maybe\"", QUOTED_STRING);
	result = process_token(&current, &i, "?", WORD);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	return result;
}
/*
 Redirection
 The overall format used for redirection is:

[n]redir-op word
The number n is an optional decimal number designating the file descriptor number; 
the application shall ensure it is delimited from any preceding text and immediately 
precede the redirection operator redir-op. If n is quoted, 
the number shall not be recognized as part of the redirection expression. 
For example:
echo \2>a
writes the character 2 into file a. If any part of redir-op is quoted, 
no redirection expression is recognized. 
For example:
echo 2\>a 
writes the characters 2>a to standard output. 
The optional number, redirection operator, and word shall not appear in the 
arguments provided to the command to be executed (if any).
*/
const char *test_scanner_identifiers5() {
	std::string str = "echo \\2>a echo 2\\>a echo 2>a";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "echo", WORD);
	result = process_token(&current, &i, "\\2", WORD);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "echo", WORD);
	result = process_token(&current, &i, "2\\>a", WORD);
	result = process_token(&current, &i, "echo", WORD);
	result = process_token(&current, &i, "2", IO_NUMBER);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "a", WORD);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	return result;
}

/*

*/
const char *test_scanner_identifiers6() 
{
	std::string str = "false && echo foo || echo bar ; false&&echo foo||echo bar";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "false", FALSETOK);
	result = process_token(&current, &i, "&&", AND_IF);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "foo", WORD);
	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "bar", WORD);
	result = process_token(&current, &i, ";", SEMI);
	result = process_token(&current, &i, "false", FALSETOK);
	result = process_token(&current, &i, "&&", AND_IF);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "foo", WORD);
	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "bar", WORD);	

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	return result;
}

/*

*/
const char *test_scanner_identifiers7() 
{
	std::string str = "true || echo foo && echo bar ; true||echo foo&&echo bar";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "foo", WORD);
	result = process_token(&current, &i, "&&", AND_IF);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "bar", WORD);
	result = process_token(&current, &i, ";", SEMI);
	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "foo", WORD);
	result = process_token(&current, &i, "&&", AND_IF);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "bar", WORD);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	return result;
}

/*
This is the entry point for the tests
*/
const char *test_scanner_true_false() 
{
	std::string str = "true false";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, "false", FALSETOK);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	return result;
}

/*
This is the entry point for the tests
*/
const char *test_scanner_true_false2() 
{
	std::string str = "true;false";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, ";", SEMI);
	result = process_token(&current, &i, "false", FALSETOK);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	return result;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_scanner_identifiers);	
	run_test(test_scanner_identifiers2);
	run_test(test_scanner_identifiers3);
	run_test(test_scanner_identifiers4);

	// run_test(test_scanner_identifiers5);
	run_test(test_scanner_identifiers6);
	run_test(test_scanner_identifiers7);

	run_test(test_scanner_true_false);
	run_test(test_scanner_true_false2);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
