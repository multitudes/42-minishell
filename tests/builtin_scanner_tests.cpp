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
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "echo hello world";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "hello", WORD);
	result = process_token(&current, &i, "world", WORD);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_echo_n() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "echo -n hello world";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "-n", FLAGS);
	result = process_token(&current, &i, "hello", WORD);
	result = process_token(&current, &i, "world", WORD);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_cd() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cd";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
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
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cd /hello/world";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
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
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "export";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
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
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "export MY_VAR=hello";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "export", BUILTIN);
	result = process_token(&current, &i, "MY_VAR=hello", WORD);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_unset() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "unset MY_VAR";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "unset", BUILTIN);
	result = process_token(&current, &i, "MY_VAR", WORD);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}



const char* test_scanner_builtins_env() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "env";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
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
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
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
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "pwd pwdecho pwd echo";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "pwd", BUILTIN);
	result = process_token(&current, &i, "pwdecho", WORD);
	result = process_token(&current, &i, "pwd", BUILTIN);
	result = process_token(&current, &i, "echo", BUILTIN);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_builtins_dot() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = ". /bin/ls . ./bin/ls";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
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

/*
make a test for all those
	if (!(strncicmp(id, "alias", 6)) || strncicmp(id, "bg", 3) == 0 || \
	strncicmp(id, "bind", 5) == 0 || strncicmp(id, "break", 6) == 0 || \
	strncicmp(id, "builtin", 8) == 0 || strncicmp(id, "caller", 6) == 0 || \
	strncicmp(id, "command", 7) == 0 || strncicmp(id, "compgen", 7) == 0 || \
	strncicmp(id, ".", 2) == 0 || strncicmp(id, "complete", 9) == 0 || \
	strncicmp(id, "continue", 8) == 0 || strncicmp(id, "declare", 7) == 0 || \
	strncicmp(id, "dirs", 4) == 0 || strncicmp(id, "disown", 6) == 0 || \
	strncicmp(id, "enable", 6) == 0 || strncicmp(id, "eval", 4) == 0 || \
	strncicmp(id, "exec", 4) == 0 || strncicmp(id, "fc", 2) == 0 || \
	strncicmp(id, "fg", 2) == 0 || strncicmp(id, "getopts", 7) == 0 || \
	strncicmp(id, "hash", 4) == 0 || strncicmp(id, "help", 4) == 0 || \
	strncicmp(id, "history", 7) == 0 || strncicmp(id, "jobs", 4) == 0 || \
	strncicmp(id, "kill", 4) == 0 || strncicmp(id, "let", 3) == 0 || \
	strncicmp(id, "local", 5) == 0 || strncicmp(id, "logout", 6) == 0 || \
	strncicmp(id, "mapfile", 7) == 0 || strncicmp(id, "popd", 4) == 0 || \
	strncicmp(id, "printf", 6) == 0 || strncicmp(id, "pushd", 5) == 0 || \
	strncicmp(id, "read", 4) == 0 || strncicmp(id, "readonly", 8) == 0 || \
	strncicmp(id, "return", 6) == 0 || strncicmp(id, "set", 3) == 0 || \
	strncicmp(id, "shift", 5) == 0 || strncicmp(id, "shopt", 5) == 0 || \
	strncicmp(id, "source", 6) == 0 || strncicmp(id, "suspend", 7) == 0 || \
	strncicmp(id, "test", 4) == 0 || strncicmp(id, "times", 5) == 0 || \
	strncicmp(id, "trap", 4) == 0 || strncicmp(id, "type", 4) == 0 || \
	strncicmp(id, "typeset", 7) == 0 || strncicmp(id, "ulimit", 6) == 0 || \
	strncicmp(id, "umask", 5) == 0 || strncicmp(id, "unalias", 7) == 0 || \
	strncicmp(id, "wait", 4) == 0 || strncicmp(id, "readarray", 9) == 0 || \
	strncicmp(id, ":", 2) == 0 || strncicmp(id, ".", 2) == 0)
	as BUILTINS
*/
const char* test_scanner_builtins_all_not_impl() {
	std::string str = "alias bg bind break builtin caller command compgen . complete continue declare dirs disown enable eval exec fc fg getopts hash help history jobs kill let local logout mapfile popd printf pushd read readonly return set shift shopt source suspend test times trap type typeset ulimit umask unalias wait readarray : .";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "alias", BUILTIN);
	result = process_token(&current, &i, "bg", BUILTIN);
	result = process_token(&current, &i, "bind", BUILTIN);
	result = process_token(&current, &i, "break", BUILTIN);
	result = process_token(&current, &i, "builtin", BUILTIN);
	result = process_token(&current, &i, "caller", BUILTIN);
	result = process_token(&current, &i, "command", BUILTIN);
	result = process_token(&current, &i, "compgen", BUILTIN);
	result = process_token(&current, &i, ".", BUILTIN);
	result = process_token(&current, &i, "complete", BUILTIN);
	result = process_token(&current, &i, "continue", BUILTIN);
	result = process_token(&current, &i, "declare", BUILTIN);
	result = process_token(&current, &i, "dirs", BUILTIN);
	result = process_token(&current, &i, "disown", BUILTIN);
	result = process_token(&current, &i, "enable", BUILTIN);
	result = process_token(&current, &i, "eval", BUILTIN);
	result = process_token(&current, &i, "exec", BUILTIN);
	result = process_token(&current, &i, "fc", BUILTIN);
	result = process_token(&current, &i, "fg", BUILTIN);
	result = process_token(&current, &i, "getopts", BUILTIN);
	result = process_token(&current, &i, "hash", BUILTIN);
	result = process_token(&current, &i, "help", BUILTIN);
	result = process_token(&current, &i, "history", BUILTIN);
	result = process_token(&current, &i, "jobs", BUILTIN);
	result = process_token(&current, &i, "kill", BUILTIN);
	result = process_token(&current, &i, "let", BUILTIN);
	result = process_token(&current, &i, "local", BUILTIN);
	result = process_token(&current, &i, "logout", BUILTIN);
	result = process_token(&current, &i, "mapfile", BUILTIN);
	result = process_token(&current, &i, "popd", BUILTIN);
	result = process_token(&current, &i, "printf", BUILTIN);
	result = process_token(&current, &i, "pushd", BUILTIN);
	result = process_token(&current, &i, "read", BUILTIN);
	result = process_token(&current, &i, "readonly", BUILTIN);
	result = process_token(&current, &i, "return", BUILTIN);
	result = process_token(&current, &i, "set", BUILTIN);
	result = process_token(&current, &i, "shift", BUILTIN);
	result = process_token(&current, &i, "shopt", BUILTIN);	
	result = process_token(&current, &i, "source", BUILTIN);
	result = process_token(&current, &i, "suspend", BUILTIN);
	result = process_token(&current, &i, "test", BUILTIN);
	result = process_token(&current, &i, "times", BUILTIN);
	result = process_token(&current, &i, "trap", BUILTIN);
	result = process_token(&current, &i, "type", BUILTIN);
	result = process_token(&current, &i, "typeset", BUILTIN);
	result = process_token(&current, &i, "ulimit", BUILTIN);
	result = process_token(&current, &i, "umask", BUILTIN);
	result = process_token(&current, &i, "unalias", BUILTIN);
	result = process_token(&current, &i, "wait", BUILTIN);
	result = process_token(&current, &i, "readarray", BUILTIN);
	result = process_token(&current, &i, ":", BUILTIN);
	result = process_token(&current, &i, ".", BUILTIN);
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
	run_test(test_scanner_builtins_all_not_impl);
	
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
