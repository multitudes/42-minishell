#include "razorclam_tests.h"
#include <string>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/scanner.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));

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
	debug("type %d = expected_type: %d", token->type, expected_tokentype);
    debug("token lexeme: -%s- ", token->lexeme);
    my_assert(strcmp(token->lexeme, expected_lexeme) == 0, "token lexeme is not expected");
    my_assert(token->type == expected_tokentype, "token type is not the expected type...");

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
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "hello", WORD);
	result = process_token(&current, &i, "world", WORD);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);
	return result;
}


const char* test_scanner_identifiers2() {

	std::string str = "echo he_-3llo world";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "he_-3llo", WORD);	
	result = process_token(&current, &i, "world", WORD);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);	

	ft_lstclear(&lexemes, free_tokennode);
	return result;
}


const char* test_scanner_identifiers3() {
	std::string str = "||\"\"";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, "\"\"", QUOTED_STRING);	
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);	
	
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

const char* test_scanner_identifiers4() {
	std::string str = "this is a \"quoted\'string\'maybe\"?";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
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
	
	ft_lstclear(&lexemes, free_tokennode);

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
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "\\2", WORD);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "2\\", WORD);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "a", WORD);
	result = process_token(&current, &i, "echo", BUILTIN);
	result = process_token(&current, &i, "2", IO_NUMBER);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "a", WORD);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

/*

*/
const char *test_scanner_identifiers6() 
{
	std::string str = "false && echo foo || echo bar ; false&&echo foo||echo bar";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
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
	
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

/*

*/
const char *test_scanner_identifiers7() 
{
	std::string str = "true || echo foo && echo bar ; true||echo foo&&echo bar";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
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
	
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

const char *test_scanner_true_false() 
{
	std::string str = "true false";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, "false", FALSETOK);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}


const char *test_scanner_true_false2() 
{
	std::string str = "true;false";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, ";", SEMI);
	result = process_token(&current, &i, "false", FALSETOK);
	// this is how I check for the end of the list
	result = process_token(&current, &i, NULL, NULL_TOKEN);
	
	ft_lstclear(&lexemes, free_tokennode);
	return result;
}

const char* test_all_tokens() {
    std::string str = "hello world 42 builtin -f /path/to/file | |& && || ;";
    const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
    t_list *current = lexemes;
    const char *result;
    int i = 0;

    // Add calls to process_token for each token in the order they appear in the string
    result = process_token(&current, &i, "hello", WORD);
    result = process_token(&current, &i, "world", WORD);
	result = process_token(&current, &i, "42", NUMBER);
	result = process_token(&current, &i, "builtin", BUILTIN);
	result = process_token(&current, &i, "-f", FLAGS);
	result = process_token(&current, &i, "/path/to/file", PATHNAME);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "|&", PIPE_AND);
	result = process_token(&current, &i, "&&", AND_IF);
	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, ";", SEMI);
	
	// this is how I check for the end of the list  
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);
    return result;
}


const char* test_all_tokens2() {
    std::string str = "& ;& ;; ;;& ( command ) $(command) ${param} $param $((expr)) \"string\" 'string' true false > < >> << delimiter >| &> >& <& &>> = <> <<- , . - + / * ";
    const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
    t_list *current = lexemes;
    const char *result;
    int i = 0;

    // Add calls to process_token for each token in the order they appear in the string

	result = process_token(&current, &i, "&", AND_TOK);
	result = process_token(&current, &i, ";&", SEMI_AND);
	result = process_token(&current, &i, ";;", DSEMI);
	result = process_token(&current, &i, ";;&", DSEMI_AND);
	result = process_token(&current, &i, "( command )", EXPRESSION);
	result = process_token(&current, &i, "$(command)", COM_EXPANSION);
	result = process_token(&current, &i, "${param}", VAR_EXPANSION);
	result = process_token(&current, &i, "$param", VAR_EXPANSION);
	result = process_token(&current, &i, "$((expr))", EXPR_EXPANSION);
	result = process_token(&current, &i, "\"string\"", QUOTED_STRING);
	result = process_token(&current, &i, "'string'", S_QUOTED_STRING);

	result = process_token(&current, &i, "true", TRUETOK);
	result = process_token(&current, &i, "false", FALSETOK);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "<", REDIRECT_IN);
	result = process_token(&current, &i, ">>", DGREAT);
	result = process_token(&current, &i, "<<", DLESS);
	result = process_token(&current, &i, "delimiter", DLESS_DELIM);

	result = process_token(&current, &i, ">|", CLOBBER);
	result = process_token(&current, &i, "&>", REDIRECT_BOTH);
	result = process_token(&current, &i, ">&", GREATAND);
	result = process_token(&current, &i, "&>", REDIRECT_BOTH);
	result = process_token(&current, &i, ">&", REDIRECT_BOTH);
	result = process_token(&current, &i, "<&", LESSAND);
	result = process_token(&current, &i, "&>>", REDIRECT_BOTH_APP);
	result = process_token(&current, &i, "=", EQUAL);
	result = process_token(&current, &i, "<>", LESSGREAT);
	result = process_token(&current, &i, "<<-", DLESSDASH);
	result = process_token(&current, &i, ",", COMMA);
	result = process_token(&current, &i, ".", BUILTIN);
	result = process_token(&current, &i, "-", MINUS);
	result = process_token(&current, &i, "+", PLUS);
	result = process_token(&current, &i, "/", PATHNAME);
	result = process_token(&current, &i, "*", GLOBBING);
	
	
	// this is how I check for the end of the list  
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);
    return result;
}


const char* test_all_tokens3() {
    std::string str = "!= ! !! 0-9 !a !?a !# == >= <= -- ++ -= += /= *= ";
    const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
    t_list *current = lexemes;
    const char *result;
    int i = 0;

    // Add calls to process_token for each token in the order they appear in the string

	result = process_token(&current, &i, "!=", BANG_EQUAL);
	result = process_token(&current, &i, "!", BANG);
	result = process_token(&current, &i, "!!", BANG_BANG);
	result = process_token(&current, &i, "0-9", WORD);
	result = process_token(&current, &i, "!a", BANG_ALPHA);
	result = process_token(&current, &i, "!?a", BANG_QUESTION_ALPHA);
	result = process_token(&current, &i, "!#", BANG_HASH);
	result = process_token(&current, &i, "=", EQUAL);
	result = process_token(&current, &i, "=", EQUAL);
	result = process_token(&current, &i, ">=", GREATER_EQUAL);
	result = process_token(&current, &i, "<=", LESS_EQUAL);
	result = process_token(&current, &i, "--", MINUS_MINUS);
	result = process_token(&current, &i, "++", PLUS_PLUS);
	result = process_token(&current, &i, "-=", MINUS_EQUAL);
	result = process_token(&current, &i, "+=", PLUS_EQUAL);
	result = process_token(&current, &i, "/=", SLASH_EQUAL);
	result = process_token(&current, &i, "*=", STAR_EQUAL);
	
	// this is how I check for the end of the list  
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);
    return result;
}


const char* test_all_tokens4() {
    std::string str = "& if then else elif fi do done in while until for case esac select function $? $$ $* $@ $# $! $- $0 $ $a # comment ^ % ~ EOF command";
    const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
    t_list *current = lexemes;
    const char *result;
    int i = 0;

    // Add calls to process_token for each token in the order they appear in the string

	result = process_token(&current, &i, "&", AND_TOK);
	result = process_token(&current, &i, "if", IF);
	result = process_token(&current, &i, "then", THEN);
	result = process_token(&current, &i, "else", ELSE);
	result = process_token(&current, &i, "elif", ELIF);
	result = process_token(&current, &i, "fi", FI);
	result = process_token(&current, &i, "do", DO);
	result = process_token(&current, &i, "done", DONE);
	result = process_token(&current, &i, "in", IN);	
	result = process_token(&current, &i, "while", WHILE);
	result = process_token(&current, &i, "until", UNTIL);
	result = process_token(&current, &i, "for", FOR);
	result = process_token(&current, &i, "case", CASE);
	result = process_token(&current, &i, "esac", ESAC);
	result = process_token(&current, &i, "select", SELECT);
	result = process_token(&current, &i, "function", FUNCTION);
	result = process_token(&current, &i, "$?", DOLLAR_QUESTION);
	result = process_token(&current, &i, "$$", DOLLAR_DOLLAR);
	result = process_token(&current, &i, "$*", DOLLAR_STAR);
	result = process_token(&current, &i, "$@", DOLLAR_AT);
	result = process_token(&current, &i, "$#", DOLLAR_HASH);
	result = process_token(&current, &i, "$!", DOLLAR_BANG);
	result = process_token(&current, &i, "$-", DOLLAR_HYPHEN);
	result = process_token(&current, &i, "$0", DOLLAR_DIGIT);
	result = process_token(&current, &i, "$", DOLLAR);
	result = process_token(&current, &i, "$a", VAR_EXPANSION);

	// comments are ignored!!!

	// this is how I check for the end of the list  
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);
    return result;
}

// test ha! ha? etc
const char* test_all_tokens5() {
    std::string str = "ha! ha? ha. ha, ha: ha; ha& ha| ha&& ha|| ha# ha% ha^ ha~ ha ha= ha< ha> ha ( ha) ha [ ha] ha{ ha} ";// ha' ha ha/ ha* ha+ ha- ha_ ha@ ha ";
    const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
    t_list *current = lexemes;
    const char *result;
    int i = 0;

    // Add calls to process_token for each token in the order they appear in the string
	result = process_token(&current, &i, "ha!", WORD);
	result = process_token(&current, &i, "ha?", WORD);
	result = process_token(&current, &i, "ha.", WORD);
	result = process_token(&current, &i, "ha,", WORD);
	result = process_token(&current, &i, "ha:", WORD);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, ";", SEMI);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, "&", AND_TOK);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, "|", PIPE);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, "&&", AND_IF);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, "||", OR_IF);
	result = process_token(&current, &i, "ha#", WORD);
	result = process_token(&current, &i, "ha%", WORD);
	result = process_token(&current, &i, "ha^", WORD);
	result = process_token(&current, &i, "ha~", WORD);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, "ha=", WORD);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, "<", REDIRECT_IN);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, ">", REDIRECT_OUT);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, "( ha)", EXPRESSION);
	result = process_token(&current, &i, "ha", WORD);
	result = process_token(&current, &i, "[", WORD);
	result = process_token(&current, &i, "ha]", WORD);
	result = process_token(&current, &i, "ha{", WORD);
	result = process_token(&current, &i, "ha}", WORD);
	
	// this is how I check for the end of the list  
	result = process_token(&current, &i, NULL, NULL_TOKEN);

	ft_lstclear(&lexemes, free_tokennode);

    return result;
}

// test globbing tokens value 96 like * 8*8 *8 dd*d *.c *.h *.o *.a *.so *.so.* *.so.1 *.so.1.2 *.so.1.2.3

const char *test_all_tokens6() {
	std::string str = "* 8*8 *8 dd*d *.c *.h *.o *.a *.so *.so.* *.so.1 *.so.1.2 *.so.1.2.3";
	const char* input = str.c_str();
	t_list *lexemes = tokenizer(input);
	t_list *current = lexemes;
	const char *result;
	int i = 0;

	// Add calls to process_token for each token in the order they appear in the string
	result = process_token(&current, &i, "*", GLOBBING);
	result = process_token(&current, &i, "8*8", GLOBBING);
	result = process_token(&current, &i, "*8", GLOBBING);
	result = process_token(&current, &i, "dd*d", GLOBBING);
	result = process_token(&current, &i, "*.c", GLOBBING);
	result = process_token(&current, &i, "*.h", GLOBBING);
	result = process_token(&current, &i, "*.o", GLOBBING);
	result = process_token(&current, &i, "*.a", GLOBBING);
	result = process_token(&current, &i, "*.so", GLOBBING);
	result = process_token(&current, &i, "*.so.*", GLOBBING);
	result = process_token(&current, &i, "*.so.1", GLOBBING);
	result = process_token(&current, &i, "*.so.1.2", GLOBBING);
	result = process_token(&current, &i, "*.so.1.2.3", GLOBBING);
	
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
	run_test(test_scanner_identifiers);	
	run_test(test_scanner_identifiers2);
	run_test(test_scanner_identifiers3);
	run_test(test_scanner_identifiers4);
	run_test(test_scanner_identifiers5);
	run_test(test_scanner_identifiers6);
	run_test(test_scanner_identifiers7);

	run_test(test_scanner_true_false);
	run_test(test_scanner_true_false2);
	run_test(test_all_tokens);
	run_test(test_all_tokens2);
	run_test(test_all_tokens3);
	run_test(test_all_tokens4);
	run_test(test_all_tokens5);
	run_test(test_all_tokens6);

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
