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
    my_assert(token->type == expected_tokentype, "token type is not IDENTIFIER");
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

*/
const char* test_scanner_identifiers() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "hello world";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "hello", IDENTIFIER);
	result = process_token(&current, &i, "world", IDENTIFIER);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}


const char* test_scanner_identifiers2() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "echo he_-3llo world";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	int i = 0;

	t_token *token = (t_token *)current->content;
	debug("token number %d ", i);
	debug("token type: %d ", token->type);
	debug("token lexeme: %s ", token->lexeme);
	my_assert(token->type == IDENTIFIER, "token type is not IDENTIFIER");
	my_assert(strcmp(token->lexeme, "echo") == 0, "token lexeme is not echo");
	current = current->next;
	i++;
	token = (t_token *)current->content;
	debug("token number %d ", i);
	debug("token type: %d ", token->type);
	debug("token lexeme: %s ", token->lexeme);
	my_assert(token->type == IDENTIFIER, "token type is not IDENTIFIER");
	my_assert(strcmp(token->lexeme, "he_-3llo") == 0, "token lexeme is not IDENTIFIER");
	current = current->next;
	i++;
	token = (t_token *)current->content;
	debug("token number %d ", i);
	debug("token type: %d ", token->type);
	debug("token lexeme: %s ", token->lexeme);
	my_assert(token->type == IDENTIFIER, "token type is not IDENTIFIER");
	my_assert(strcmp(token->lexeme, "world") == 0, "token lexeme is not IDENTIFIER");	

	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_scanner_identifiers);	
	// run_test(test_scanner_identifiers2);
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
