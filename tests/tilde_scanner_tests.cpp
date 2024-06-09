#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/scanner.h"

/*
testing for TILDE character in the scanner

*/
const char* process_token(t_list **current, int *i, const char* expected_lexeme, t_tokentype expected_tokentype) 
{
	// first check if at the end of the list to avoid segfault
	if (*current == NULL && expected_lexeme == NULL && expected_tokentype == NULL_TOKEN) 
		return NULL;
	
    t_token *token = (t_token *)(*current)->content;
	
    debug("\ngot token number %d ", *i);
    debug("got token type: %d ", token->type);
    debug("got token lexeme: -%s- \n", token->lexeme);
    my_assert(token->type == expected_tokentype, "token type is not the expected type..." );
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
const char* test_tilde_tokens() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "~ ~~ /home/username ~/Documents ~/Desktop ";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;

	result = process_token(&current, &i, "~", TILDE);
	result = process_token(&current, &i, "~~", WORD);
	result = process_token(&current, &i, "/home/username", PATHNAME);
	result = process_token(&current, &i, "~/Documents", PATHNAME);
	result = process_token(&current, &i, "~/Desktop", PATHNAME);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_tilde_tokens2() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "~- ~-/ ~-/notadirectory ~-qwerty ~-qwerty/ ~-qwerty/123 ";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;

	result = process_token(&current, &i, "~-", PATHNAME);
	result = process_token(&current, &i, "~-/", PATHNAME);
	result = process_token(&current, &i, "~-/notadirectory", PATHNAME);
	result = process_token(&current, &i, "~-qwerty", WORD);
	result = process_token(&current, &i, "~-qwerty/", PATHNAME);
	result = process_token(&current, &i, "~-qwerty/123", PATHNAME);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}


/*
same as above but with ~+
*/
const char * test_tilde_tokens3() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "~+ ~+~/ ~+/home/username ~+/Documents ~+/Desktop ~+qwerty ~+qwerty/ ~+qwerty/123";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;

	result = process_token(&current, &i, "~+", PATHNAME);
	result = process_token(&current, &i, "~+~/", PATHNAME);
	result = process_token(&current, &i, "~+/home/username", PATHNAME);
	result = process_token(&current, &i, "~+/Documents", PATHNAME);
	result = process_token(&current, &i, "~+/Desktop", PATHNAME);
	result = process_token(&current, &i, "~+qwerty", WORD);
	result = process_token(&current, &i, "~+qwerty/", PATHNAME);
	result = process_token(&current, &i, "~+qwerty/123", PATHNAME);
	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_tilde_tokens);
	run_test(test_tilde_tokens2);
	run_test(test_tilde_tokens3);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
