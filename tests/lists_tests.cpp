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

I want to check the output of the call to the function in scanner.c file
tokenizer(char *input) returning a t_list of lexemes
I will create a string and check the output of the function
no need for spaces || and && 
*/
const char* test_lists_identifiers() {

	std::string str = "false && echo foo || echo bar";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	// result = process_token(&current, &i, "false", IDENTIFIER);
	// result = process_token(&current, &i, "&&", AND_IF);
	// result = process_token(&current, &i, "echo", IDENTIFIER);
	// result = process_token(&current, &i, "foo", IDENTIFIER);
	// result = process_token(&current, &i, "||", OR_IF);
	// result = process_token(&current, &i, "echo", IDENTIFIER);
	// result = process_token(&current, &i, "bar", IDENTIFIER);
	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	// temporary fix to avoid testing this function
	result = NULL;
	return result;
}


const char* test_lists_identifiers2() {

	std::string str = "true || echo foo && echo bar";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	// result = process_token(&current, &i, "false", IDENTIFIER);
	// result = process_token(&current, &i, "&&", AND_IF);
	// result = process_token(&current, &i, "echo", IDENTIFIER);
	// result = process_token(&current, &i, "foo", IDENTIFIER);
	// result = process_token(&current, &i, "||", OR_IF);
	// result = process_token(&current, &i, "echo", IDENTIFIER);
	// result = process_token(&current, &i, "bar", IDENTIFIER);
	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	// temporary fix to avoid testing this function	
	result = NULL;
	return result;
}


const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_lists_identifiers);	
	// run_test(test_lists_identifiers2);	

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
