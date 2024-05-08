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

/*
This is a test function. It should return NULL if the test passes
debug will print to the log
log_err will print to the error log
my_assert will check a condition and return an error message if it fails
run_test will run a test function and return the error message if it fails
RUN_TESTS will run all the tests and print the results

*/
const char* test_scanner_redirections() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "a>file.txt a>|file.txt a>&file.txt a&>file.txt a>>file.txt ls&>>file.txt";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;

	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "file.txt", WORD);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, ">|", CLOBBER);
	result = process_token(&current, &i, "file.txt", WORD);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, ">&", GREATAND);
	result = process_token(&current, &i, "file.txt", WORD);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "&>", REDIRECT_BOTH);
	result = process_token(&current, &i, "file.txt", WORD);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, ">>", DGREAT);
	result = process_token(&current, &i, "file.txt", WORD);
	result = process_token(&current, &i, "ls", WORD);
	result = process_token(&current, &i, "&>>", REDIRECT_OUT_APP);
	result = process_token(&current, &i, "file.txt", WORD);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char* test_scanner_redirections2() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cat<file.txt cat< file cat < file cat <file";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "<", REDIRECT_IN);
	result = process_token(&current, &i, "file.txt", WORD);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "<", REDIRECT_IN);
	result = process_token(&current, &i, "file", WORD);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "<", REDIRECT_IN);
	result = process_token(&current, &i, "file", WORD);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "<", REDIRECT_IN);
	result = process_token(&current, &i, "file", WORD);


	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

/*
more redirection -> expression ( "<" | ">" | ">>" | ">>&" | "2>" | "&> | &>> | 2>> | <> | >|") expression; 

*/
const char* test_scanner_redirections3() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cat<cat>cat>>cat>>&cat";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "<", REDIRECT_IN);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, ">>", DGREAT);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, ">>&", REDIRECT_BOTH_APP);
	result = process_token(&current, &i, "cat", WORD);


	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}


/*
more redirection -> expression ( "<" | ">" | ">>" | ">>&" | "2>" | "&> | &>> | 2>> | <> | >|") expression; 

*/
const char* test_scanner_redirections4() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cat2>cat2>&cat&>cat>&cat&>>cat2>>cat<>cat>|cat";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;

	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "2", IO_NUMBER);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "2", IO_NUMBER);
	result = process_token(&current, &i, ">&", GREATAND);	
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "&>", REDIRECT_BOTH);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, ">&", GREATAND);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "&>>", REDIRECT_OUT_APP);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "2", IO_NUMBER);
	result = process_token(&current, &i, ">>", DGREAT);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "<>", LESSGREAT);
	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, ">|", CLOBBER);
	result = process_token(&current, &i, "cat", WORD);

	//this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

/*
cat << EOF | grep 'foo'
> This is a line.
> This is another line.
> This line contains foo.
> EOF
*/
const char* test_scanner_append() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cat << EOF | grep 'foo' \n This is a line. \n This is another line. \n This line contains foo. \n EOF";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "<<", DLESS);
	result = process_token(&current, &i, "EOF", DLESS_DELIM);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "grep", WORD);
	result = process_token(&current, &i, "'foo'", S_QUOTED_STRING);
	result = process_token(&current, &i, "This", WORD);
	result = process_token(&current, &i, "is", WORD);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "line.", WORD);
	result = process_token(&current, &i, "This", WORD);
	result = process_token(&current, &i, "is", WORD);
	result = process_token(&current, &i, "another", WORD);
	result = process_token(&current, &i, "line.", WORD);
	result = process_token(&current, &i, "This", WORD);
	result = process_token(&current, &i, "line", WORD);
	result = process_token(&current, &i, "contains", WORD);
	result = process_token(&current, &i, "foo.", WORD);
	result = process_token(&current, &i, "EOF", WORD);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

/*
they do not need a space bef or after as long as they have a delimiter string
after <<
*/
const char* test_scanner_append2() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "cat<<EOF<<foo";
	const char* input = str.c_str();
	init_data(&g_mini_data);
	g_mini_data->input = input;
	t_list *lexemes = tokenizer(g_mini_data);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "cat", WORD);
	result = process_token(&current, &i, "<<", DLESS);
	result = process_token(&current, &i, "EOF", DLESS_DELIM);
	result = process_token(&current, &i, "<<", DLESS);
	result = process_token(&current, &i, "foo", DLESS_DELIM);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	return result;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_scanner_redirections);	
	run_test(test_scanner_redirections2);
	run_test(test_scanner_redirections3);
	run_test(test_scanner_redirections4);
	run_test(test_scanner_append);
	run_test(test_scanner_append2);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
