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

• Your shell must implement the following builtins:
◦ echo with option -n
◦ echo without any options
◦ cd with only a relative or absolute path 
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options

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

/*
This is a test function. It should return NULL if the test passes
debug will print to the log
log_err will print to the error log
my_assert will check a condition and return an error message if it fails
run_test will run a test function and return the error message if it fails
RUN_TESTS will run all the tests and print the results

*/
const char* test_scanner_builtins_echo() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "echo hello world";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "hello", IDENTIFIER);
	result = process_token(&current, &i, "world", IDENTIFIER);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_echo_n() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "echo -n hello world";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "-n", FLAGS);
	result = process_token(&current, &i, "hello", IDENTIFIER);
	result = process_token(&current, &i, "world", IDENTIFIER);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_cd() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cd";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "cd", BUILTIN);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_cd_dir() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cd /hello/world";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "cd", BUILTIN);
	result = process_token(&current, &i, "/hello/world", PATHNAME);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_export() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "export";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "export", BUILTIN);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_export_var() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "export MY_VAR=hello";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "export", BUILTIN);
	result = process_token(&current, &i, "MY_VAR=hello", IDENTIFIER);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_unset() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "unset MY_VAR";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "unset", BUILTIN);
	result = process_token(&current, &i, "MY_VAR", IDENTIFIER);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}



const char* test_scanner_builtins_env() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "env";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "env", BUILTIN);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}


const char* test_scanner_builtins_exit() {
	std::string str = "exit";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "exit", BUILTIN);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_pwd() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "pwd pwdecho pwd echo";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "pwd", BUILTIN);
	result = process_token(&current, &i, "pwdecho", IDENTIFIER);
	result = process_token(&current, &i, "pwd", BUILTIN);
	result = process_token(&current, &i, "echo", BUILTIN);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_dot() {
	
	// i want to check the output of the call to the function in scanner.c file
	// scan_this(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = ". /bin/ls . ./bin/ls";
	const char* input = str.c_str();
	t_list *lexemes = scan_this(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, ".", BUILTIN);
	result = process_token(&current, &i, "/bin/ls", PATHNAME);
	result = process_token(&current, &i, ".", BUILTIN);
	result = process_token(&current, &i, "./bin/ls", PATHNAME);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}



const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_scanner_builtins_echo);	
	run_test(test_scanner_builtins_echo_n);
	run_test(test_scanner_builtins_cd);
	run_test(test_scanner_builtins_cd_dir);
	run_test(test_scanner_builtins_export);	
	run_test(test_scanner_builtins_export_var);	
	run_test(test_scanner_builtins_unset);
	run_test(test_scanner_builtins_env);
	run_test(test_scanner_builtins_exit);
	run_test(test_scanner_builtins_pwd);
	run_test(test_scanner_builtins_dot);
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
