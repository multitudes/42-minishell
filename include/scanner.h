/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:55:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/06 11:52:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

// needed for the tests - leave it here
#ifdef __cplusplus
extern "C" {
#endif

# include "minishell.h"

/*
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

control operator
A token that performs a control function. It is a newline or one of the following: ‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.
*/
typedef enum e_tokentype {
	IDENTIFIER, 
	// Any sequence of letters, digits, and underscores
	//| and |& have higher precedence than &&, ||, ;, and &. 
	// They allow you to create pipelines, where the output of 
	// one command is used as the input of the next command (|), 
	//or where both the output and error output of one command are 
	// used as the input of the next command (|&).
	NUMBER, 
	STRING, 
	BUILTIN,
	FLAGS,
	PATHNAME, 
	PIPE, // |
	PIPE_AND, // |&
	AND_TOK, // &
	//&& and || have the same precedence and are left-associative. 
	//They allow you to execute a command based on the success 
	//(&&) or failure (||) of the previous command.
	AND_IF, 	// &&
	OR_IF, // || 
	//; and & have the same precedence, which is lower than && 
	//and ||. They allow you to separate commands (;) 
	//or run a command in the background (&).
	SEMI, 
	SEMI_AND,
	// ;;, ;&, and ;;& are used in the context of a case 
	// statement to separate different cases.
 	DSEMI, 
	DSEMI_AND,
	EXPRESSION,
	// Command expansion or substitution
    COM_EXPANSION, // '$(command)' or '`command`'
	VAR_EXPANSION,  EXPR_EXPANSION, // ${parameter} or $parameter or $(command) or $((arythm expression))
	
	QUOTED_STRING, // quoted string "string" 
	SINGLE_QUOTED_STRING, // quoted string 'string'
	// ( and ) can be used to group commands, 
	// which can override the default precedence rules.
	
	LEFT_PAREN, RIGHT_PAREN, 
	LEFT_SQUARE, RIGHT_SQUARE,
 	LEFT_CURLY, RIGHT_CURLY,

    // Redirection operators
	IO_NUMBER, // 'n>' or 'n>>' or 'n>&' or 'n<&' or 'n<>' a numberfollowed by a redirection operator < or >
    REDIRECT_OUT, // '>'
	CLOBBER, // '>|'
	REDIRECT_BOTH, // '&>'
	GREATAND, // '>&'
    // REDIRECT_ERR, // '2>'
	// REDIRECT_ERR_FD, //2>&

    REDIRECT_IN, // '<'
	LESSAND, // '<&'
    DGREAT, // '>>'
	REDIRECT_OUT_APP, // '&>>'	
	DLESS_DELIM,
	// REDIRECT_ERR_APP, //2>>
	REDIRECT_BOTH_APP, //">>&"
 
    DLESS, // '<<'
	
	//The <> operator in bash is used for opening a file in read-write mode. Here's an example:
	// command <> file.txt
	LESSGREAT, 		// '<>'
	DLESSDASH,	 // '<<-' The here-document delimiter is treated as a literal string,
	GREATER_AND_GREATER, 
	COMMA, DOT, MINUS, PLUS, SLASH, STAR,
	// One or two character tokens.
	BANG_EQUAL,	BANG,
	// for the history expansion
	BANG_BANG,BANG_DIGIT, BANG_HYPHEN_DIGIT, BANG_ALPHA, BANG_QUESTION_ALPHA,

	EQUAL, EQUAL_EQUAL,
	GREATER_EQUAL, LESS_EQUAL, MINUS_MINUS, PLUS_PLUS,
	MINUS_EQUAL, PLUS_EQUAL, SLASH_EQUAL, STAR_EQUAL,
	// Keywords.

	// control operators
	NEWLINE_TOK, // A newline
	// metacharacter : A character that, when unquoted, separates words. 
	// A metacharacter is a space, tab, newline, or: ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
	SPACE_TOK, TAB_TOK, GREATER_AND, 
	AMPERSAND,

    // reserved Keywords
	
    IF, THEN, ELSE, ELIF, FI, DO, DONE, IN,
	WHILE, UNTIL, FOR, CASE, 
	ESAC, SELECT, FUNCTION,
    // Special variables
    DOLLAR_QUESTION, // '$?'  The special parameter ‘?’ is used to get the exit status of the last command.
    DOLLAR_DOLLAR, // '$$' ‘$’ is used to get the process ID of the shell.
    DOLLAR_STAR, // '$*' ‘*’ is used to get all the positional parameters.
    DOLLAR_AT, // '$@'  ‘@’ is used to get all the positional parameters, except for the zeroth positional parameter.
    DOLLAR_HASH, // '$#'  ‘#’ is used to get the number of positional parameters.
    DOLLAR_BANG, // '$!'  ‘!’ is used to get the process ID of the last background command.
	DOLLAR_HYPHEN, // '$-' used to get the current options set for the shell.	 
	DOLLAR_DIGIT, // '$0' ‘0’ is used to get the name of the shell or script.
	DOLLAR,

	QUOTE, SINGLE_QUOTE, BACKSLASH,

	DIGIT, CHAR,
	// end of file
	HASH, COMMENT,
	CARET, PERCENT, TILDE,
	EOF_TOK, 
	COMMAND,
	NULL_TOKEN
	// to add ‘$’, ‘`’, ‘"’, ‘\’,The special parameters ‘*’ and ‘@’ have special meaning when in double quotes
	
}		t_tokentype;

// I will create a struct to keep track of all these
struct s_open_close {
	int paren;
	int curlys;
	int quotes;
	int single_quotes;
	int backslash;
	int square_brackets;	
};
typedef struct s_open_close t_open_close;

struct s_token {
	t_tokentype	type;
	char		*lexeme;
	int 		start;
};
typedef struct s_token t_token;

/*
is this used?
*/
struct s_token_list {
	t_list *head;
};
typedef struct s_token_list t_token_list;

void	print_token_description(const t_list *token_list); 
t_list	*create_token(t_tokentype type, const char *lexeme, int *start);
t_list	*tokenizer(t_mini_data *data);
bool	peek(const char *input, const char *identifier, bool end_space);
bool	is_space(const char c);
int		strncicmp(char const *a, char const *b, int n);
bool	is_delimiter(const char ch);
bool	is_digit(char c); 
bool	is_alnum(char c);
bool	is_pathname(char c); 
bool	is_reserved(t_list **token_list, char *identifier,int *start);
bool 	is_builtin(t_list **token_list, char *identifier,int *start);
void	print_token_description(const t_list *current); 
void	print_token_list(t_list *token_list);
void	init_open_close(t_open_close *open_close);
int		check_paren_quotes(t_list *token_list);
t_list	*create_token(t_tokentype type, const char *lexeme, int *i);
int		str_is_number(const char *identifier);
int		str_is_alphanum(const char *identifier);
bool	filename_delimiter(const char ch);
int		isprint_no_space(const char *identifier);
bool	not_implemented_builtin(const char *identifier);

#ifdef __cplusplus
}
#endif

#endif  // SCANNER_H_
