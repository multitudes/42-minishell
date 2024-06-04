#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/scanner.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));

/*
testing for     
    DOLLAR_QUESTION, // '$?'  The special parameter ‘?’ is used to get the exit status of the last command.
    DOLLAR_DOLLAR, // '$$' ‘$’ is used to get the process ID of the shell.
    DOLLAR_STAR, // '$*' ‘*’ is used to get all the positional parameters.
    DOLLAR_AT, // '$@'  ‘@’ is used to get all the positional parameters, except for the zeroth positional parameter.
    DOLLAR_HASH, // '$#'  ‘#’ is used to get the number of positional parameters.
    DOLLAR_BANG, // '$!'  ‘!’ is used to get the process ID of the last background command.
	DOLLAR_HYPHEN, // '$-' used to get the current options set for the shell.	 
	DOLLAR_DIGIT, // '$0' ‘0’ is used to get the name of the shell or script.
they do not need to have a space before to be recognized as tokens
like
world$$ will be two tokens
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
const char* test_scanner_dollar() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "a$$ a$* a$@ a$# a$! a$- a$0 $";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "$$", DOLLAR_DOLLAR);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "$*", DOLLAR_STAR);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "$@", DOLLAR_AT);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "$#", DOLLAR_HASH);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "$!", DOLLAR_BANG);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "$-", DOLLAR_HYPHEN);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "$0", DOLLAR_DIGIT);
	result = process_token(&current, &i, "$", DOLLAR);

	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

const char* test_scanner_subst_dollar() {
	// Parameter names in bash can only contain alphanumeric 
	// characters or underscores, and must start with a letter or underscore.
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "w$(my_var)w w${my_var}w w$my_var w$ w`echo hello`w w$((2+2))w";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$(my_var)", COM_EXPANSION);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "${my_var}", VAR_EXPANSION);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$my_var", VAR_EXPANSION);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$", DOLLAR);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "`echo hello`", COM_EXPANSION);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);	
	result = process_token(&current, &i, "$((2+2))", EXPR_EXPANSION);
	result = process_token(&current, &i, "w", WORD);
	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

const char* test_scanner_subst_dollar_digit() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "w$0w w$1w w$2w w$3w w$4w w$5w w$6w w$7w w$8w w$9w $10";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$0", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$1", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$2", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$3", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$4", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$5", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$6", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$7", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$8", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$9", DOLLAR_DIGIT);
	result = process_token(&current, &i, "w", WORD);
	result = process_token(&current, &i, "$10", DOLLAR_DIGIT);

	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);

	return result;
}

/*
testing for the history expansion even if we will not implement it likely...
BANG_BANG,BANG_DIGIT, BANG_HYPHEN_DIGIT, BANG_ALPHA, BANG_QUESTION_ALPHA,

*/
const char* test_scanner_history_exp() {
	
	// i want to check the output of the call to the function in scanner.c file
	// tokenizer(char *input) returning a t_list of lexemes
	// I will create a string and check the output of the function
	std::string str = "!! !1 !-1 !a !?a? !?b";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "!!", BANG_BANG);
	result = process_token(&current, &i, "!1", BANG_DIGIT);
	result = process_token(&current, &i, "!-1", BANG_HYPHEN_DIGIT);
	result = process_token(&current, &i, "!a", BANG_ALPHA);
	result = process_token(&current, &i, "!?a?", BANG_QUESTION_ALPHA);
	result = process_token(&current, &i, "!?b", BANG_QUESTION_ALPHA);

	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);
	
	return result;
}


const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_scanner_dollar);	
	run_test(test_scanner_subst_dollar);
	run_test(test_scanner_subst_dollar_digit);
	run_test(test_scanner_history_exp);

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