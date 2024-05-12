/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:55:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/12 19:36:24 by lbrusa           ###   ########.fr       */
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
	WORD,  // Any sequence of letters, digits, and underscores
	NUMBER, // like 42 42.42 -2 etc 
	BUILTIN,
	FLAGS,
	PATHNAME, 
	PIPE, // | - | and |& have higher precedence than &&, ||, ;, and &. 
	PIPE_AND, // |&

	AND_TOK, // &
	AND_IF, 	// &&
	OR_IF, // || 
	
	//; and & have the same precedence, which is lower than && 
	//and ||. They allow you to separate commands (;) 
	//or run a command in the background (&).
	SEMI, // ;
	SEMI_AND, // ;&
	// ;;, ;&, and ;;& are used in the context of a case 
	// statement to separate different cases.
 	DSEMI, // double semi colon ;; not implemented
	DSEMI_AND, // ;;& not implemented
	EXPRESSION, // (....) will be expanded 
	COM_EXPANSION, // '$(command)' or '`command`'
	VAR_EXPANSION,  // ${parameter} or $parameter or $variable
	EXPR_EXPANSION, //$((arythm expression))
	QUOTED_STRING, // quoted string "string" 
	S_QUOTED_STRING, // single quoted string like 'string'
	// ( and ) can be used to group commands, 
	// which can override the default precedence rules.

	// to use the && and || operators the true and false will be assigned to these tokens
	TRUETOK,
	FALSETOK, 

	// maybe we dont need them ?
	// LEFT_PAREN, RIGHT_PAREN, 
	// LEFT_SQUARE, RIGHT_SQUARE,
 	// LEFT_CURLY, RIGHT_CURLY,

    // Redirection operators
	IO_NUMBER, // 'n>' or 'n>>' or 'n>&' or 'n<&' or 'n<>' a numberfollowed by a redirection operator < or >
    REDIRECT_OUT, // '>'
    REDIRECT_IN, // '<'
    DGREAT, // '>>'
    DLESS, // '<<'
	DLESS_DELIM,
	
	CLOBBER, // '>|'
	REDIRECT_BOTH, // '&>'
	GREATAND, // '>&'
	LESSAND, // '<&'
	REDIRECT_OUT_APP, // '&>>'	
	REDIRECT_BOTH_APP, //">>&"
	EQUAL, // =
 
	
	//The <> operator in bash is used for opening a file in read-write mode. Here's an example:
	// command <> file.txt
	LESSGREAT, 		// '<>'
	DLESSDASH,	 // '<<-' The here-document delimiter is treated as a literal string,
	GREATER_AND_GREATER, // ">&>",
	COMMA, MINUS, PLUS, STAR, BANG_EQUAL, BANG,
	// for the history expansion
	BANG_BANG, // !! repeat the last command history
	BANG_DIGIT, // !n repeat the nth command history
	BANG_HYPHEN_DIGIT, 
	BANG_ALPHA, 
	BANG_QUESTION_ALPHA, 
	BANG_HASH,
	
	EQUAL_EQUAL,
	GREATER_EQUAL, LESS_EQUAL, MINUS_MINUS, PLUS_PLUS,
	MINUS_EQUAL, PLUS_EQUAL, SLASH_EQUAL, STAR_EQUAL,
	// Keywords.

	// control operators
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
 

	DIGIT, CHAR,
	// end of file
	HASH, COMMENT,
	CARET, PERCENT, TILDE,
	EOF_TOK, 
	COMMAND,
	NULL_TOKEN
	// to add ‘$’, ‘`’, ‘"’, ‘\’,The special parameters ‘*’ and ‘@’ have special meaning when in double quotes
	
}		t_tokentype;



struct s_token {
	t_tokentype	type;
	char		*lexeme;
	int 		start;
};
typedef struct s_token t_token;

/*
is this used?
can we go without?
*/
struct s_token_list {
	t_list *head;
};
typedef struct s_token_list t_token_list;

t_list	*create_token(t_tokentype type, const char *lexeme, int *start);
t_list	*tokenizer(t_mini_data *data);
bool	peek(const char *input, const char *identifier, bool end_space);
void	advance(int *i);
bool	is_space(const char c);
int		strncicmp(char const *a, char const *b, int n);
bool 	cmp_char_case_insensitive(const char a, const char b);
bool	is_delimiter(const char ch);
bool	is_not_delimiter(const char ch);
bool	is_digit(const char c); 
bool	is_alnum(const char c);
bool	is_alpha(const char c);
bool	is_pathname(const char c);
bool	is_in_pathname(const char c);
bool	is_delimiting_char(const char c);
bool	is_a_pathname_or_num(t_mini_data *data, const char *tmp, int *start);
bool	is_reserved(t_mini_data *data, char *identifier,int *start);
bool	is_true_false(t_mini_data *data, char *str, int *start);
bool 	is_builtin(t_mini_data *data, char *identifier,int *start);
bool	add_token(t_mini_data *data, int *i, const char *lxm, int type);
void	print_token_list(t_list *token_list);
t_list	*create_token(t_tokentype type, const char *lexeme, int *i);
bool	is_io_number(const char *identifier);
bool	str_is_number(const char *identifier);
bool	str_is_alphanum(const char *identifier);
bool	filename_delimiter(const char ch);
int		isprint_no_space(const char *identifier);
bool	not_implemented_builtin(const char *identifier);
void	free_token(void *content);
bool	scanner_error(t_mini_data *data, char *err_str);
bool	got_tokens(t_mini_data *data, int *i);
bool	is_a_dollar_exp(t_mini_data *data, int *i);
bool	is_complex_dollar_exp(t_mini_data *data, int *i);
bool	is_simple_dollar_exp(t_mini_data *data, int *i);
bool	add_block_dbl_paren(t_mini_data *data, int *i, char *delim, int t_type);
bool	proc_token_off_1(t_mini_data *data, int *i, bool (*cnd)(char), int type);
bool	add_tokenblock(t_mini_data *data, int *i, char delim, int t_type);

#ifdef __cplusplus
}
#endif

#endif  // SCANNER_H_
