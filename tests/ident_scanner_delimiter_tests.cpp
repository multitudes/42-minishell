#include "razorclam_tests.h"
#include <string>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/scanner.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));

/*
file delimiters (for what is not accepted in file names)

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
const char* test_scanner_ident_space() {
	
	std::string str = "my_co2mmand ";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "my_co2mmand", WORD);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

const char* test_scanner_ident_allrandom() {
	
	std::string str = "w-+}[]=w{~or^%ld";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "w-+}[]=w{~or^%ld", WORD);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

const char* test_scanner_ident_comment() {
	
	std::string str = "cd #hello world";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "cd", BUILTIN);
	// comments are ignored!
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

/*
when in bash I input /#hello I get No such file or directory
so it is interpreted as pathname..
*/
const char* test_scanner_ident_comment2() {
	
	std::string str = "/#hello";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "/#hello", PATHNAME);
	// comments are ignored!
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

/*
NB a backslash here has to be written as \\ to be interpreted as a backslash
so what I pass to the shell is really \#hello and the # is escaped and 
the string is not a comment anymore!
*/
const char* test_scanner_ident_comment3() {
	
	std::string str = "#hello world";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	// result = process_token(&current, &i, "#hello world", COMMENT);
	// comments are ignored!
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

/*
a number is either an integer or a float
but this is not 42.42.42 as it is not a valid number is WORD
open for further classification eventually!
*/
const char* test_scanner_ident_number() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "42 -42 42.42 -42.42 42. -42. 42.42.42 42.42.";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "42", NUMBER);
	// this is how I check for the end of the list
	result = process_token(&current, &i, "-42", NUMBER);
	result = process_token(&current, &i, "42.42", NUMBER);
	result = process_token(&current, &i, "-42.42", NUMBER);
	result = process_token(&current, &i, "42.", NUMBER);
	result = process_token(&current, &i, "-42.", NUMBER);
	result = process_token(&current, &i, "42.42.42", WORD);
	result = process_token(&current, &i, "42.42.", WORD);
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

const char* test_scanner_ident_pathname() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "./mydir/myfile.txt /mydir/myfile.txt ./ ../ / mydir/myfile.txt";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result = NULL;
	int i = 0;

	result = process_token(&current, &i, "./mydir/myfile.txt", PATHNAME);
	result = process_token(&current, &i, "/mydir/myfile.txt", PATHNAME);
	result = process_token(&current, &i, "./", PATHNAME);
	result = process_token(&current, &i, "../", PATHNAME);
	result = process_token(&current, &i, "/", PATHNAME);
	result = process_token(&current, &i, "mydir/myfile.txt", PATHNAME);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}



const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_scanner_ident_space);	
	run_test(test_scanner_ident_allrandom);
	run_test(test_scanner_ident_comment);
	run_test(test_scanner_ident_comment2);
	run_test(test_scanner_ident_comment3);
	run_test(test_scanner_ident_number);
	run_test(test_scanner_ident_pathname);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);



//avoiding adding the whole libft only for this
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL)
		return ;
	del(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	**l;
	t_list	*temp;

	if (lst == NULL || *lst == NULL)
		return ;
	l = lst;
	temp = *lst;
	while ((*lst)->next)
	{
		*lst = (*lst)->next;
		ft_lstdelone(temp, del);
		temp = *lst;
	}
	ft_lstdelone(temp, del);
	*l = NULL;
}
