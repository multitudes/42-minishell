/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:55:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/10 17:33:40 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

# include <stdbool.h>
# include <libft.h>

/*
to make the code more clear I add these two defines 
intended to be used with the peek function
so EXACT it means I will look for the exact match
and FUZZY it means I will look for the match with anything after it 
*/

# define EXACT true
# define FUZZY false

// for testing - leave here
# ifdef __cplusplus

extern "C" {
# endif



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
For example, command 2>&1 redirects the standard error (file descriptor 2) 
to standard output (file descriptor 1).

&> 
This operator redirects both standard output and standard error 
to a file. It's equivalent to command > file 2>&1. 
For example, command &> file redirects both the standard output and standard 
error of command to file.

>> This operator appends the output of a command to a file. 
If the file doesn't exist, it is created. 
For example, echo Hello >> file.txt appends "Hello" to file.txt, 
preserving its existing contents.

control operator
A token that performs a control function. It is a newline or one of the 
following: ‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.
*/
typedef enum e_tokentype {
	WORD,
	NUMBER,
	BUILTIN,
	FLAGS,
	PATHNAME,
	PIPE,
	AND_TOK,
	AND_IF,
	OR_IF,
	EXPRESSION,
	COM_EXPANSION,
	VAR_EXPANSION,
	EXPR_EXPANSION,
	QUOTED_STRING,
	S_QUOTED_STRING,
	TRUETOK,
	FALSETOK,
	DGREAT,
	DLESS,
	DLESS_DELIM,
	REDIRECT_OUT,
	REDIRECT_IN,
	REDIRECT_BOTH,
	REDIRECT_BOTH_APP,
	GLOBBING,
	DOLLAR_QUESTION,
	TILDE,
	COMMAND,
	EXPANDED,
	QUOTE_EXPANDED,
	NULL_TOKEN,
	SEMI,
	SEMI_AND,
	DSEMI,
	DSEMI_AND,
	IO_NUMBER,
	CLOBBER,
	DLESSDASH,
	GREATAND,
	LESSAND,
	LESSGREAT,
	EQUAL,
	COMMA,
	MINUS,
	PLUS,
	BANG_EQUAL,
	BANG,
	BANG_BANG,
	BANG_DIGIT,
	BANG_HYPHEN_DIGIT,
	BANG_ALPHA,
	BANG_QUESTION_ALPHA,
	BANG_HASH,
	IF,
	THEN,
	ELSE,
	ELIF,
	FI,
	DO,
	DONE,
	IN,
	WHILE,
	UNTIL,
	FOR,
	CASE,
	ESAC,
	SELECT,
	FUNCTION,
	GREATER_EQUAL,
	LESS_EQUAL,
	MINUS_MINUS,
	PLUS_PLUS,
	MINUS_EQUAL,
	PLUS_EQUAL,
	SLASH_EQUAL,
	STAR_EQUAL,
	PIPE_AND,
	DOLLAR_DOLLAR,
	DOLLAR_STAR,
	DOLLAR_AT,
	DOLLAR_HASH,
	DOLLAR_BANG,
	DOLLAR_HYPHEN,
	DOLLAR_TILDE,
	DOLLAR,
	CARET,
	PERCENT,
} t_tokentype;

struct						s_mini_data
{
	const char				*input;
	t_list					*token_list;
	bool					scanner_error;
	char					*scanner_err_str;
};

typedef struct s_mini_data	t_mini_data;

struct					s_token
{
	t_tokentype			type;
	char				*lexeme;
	bool				folldbyspace;
}						t_token;
typedef struct s_token		t_token;

int		init_scanner_data(t_mini_data *data, const char *input);
t_list	*new_toknode(t_tokentype type, const char *lexeme, \
						int *start, bool folldbyspace);
t_list	*tokenizer(const char *input);
t_list	*string_tokenizer(const char *input);
bool	peek(const char *input, const char *identifier, bool end_space);
void	advance(int *i);
int		skip_space(t_list *tokenlist, const char *input, int *i);
bool	is_space(const char c);
int		ft_strncicmp(char const *a, char const *b, int n);
bool	cmp_char_case_insensitive(const char a, const char b);
bool	is_delimiter(const char ch);
bool	is_not_delimiter(const char ch);
bool	is_digit(const char c);
bool	is_alnum(const char c);
bool	is_alpha(const char c);
bool	char_is_in_pathname(const char c);
bool	not_a_delimiting_char(const char c);
bool	is_a_pathname_or_num(t_mini_data *data, const char *tmp, int *start);
bool	is_reserved(t_mini_data *data, char *identifier, int *start);
bool	is_true_false(t_mini_data *data, char *str, int *start);
bool	is_builtin(t_mini_data *data, char *identifier, int *start);
bool	add_token(t_mini_data *data, int *i, const char *lxm, t_tokentype type);
void	print_token_list(t_list *token_list);
bool	is_io_number(const char *identifier);
bool	str_is_number(const char *identifier);
bool	str_is_alphanum(const char *identifier);
bool	filename_delimiter(const char ch);
bool	not_implemented_builtin(const char *identifier);
void	free_tokennode(void *content);
bool	scanner_error(t_mini_data *data, char *err_str);
bool	got_tokens(t_mini_data *data, int *i);
bool	is_a_dollar_exp(t_mini_data *data, int *i);
bool	is_complex_dollar_exp(t_mini_data *data, int *i);
bool	is_simple_dollar_exp(t_mini_data *data, int *i);
bool	add_block_dbl_paren(t_mini_data *data, int *i, char *delim, int t_type);
bool	proc_token_off_1(t_mini_data *data, int *i, bool (*cnd)(char), int typ);
bool	add_tokenblock(t_mini_data *data, int *i, char delim, int t_type);
bool	is_a_math_op(t_mini_data *data, int *i);
bool	is_a_control_operator(t_mini_data *data, int *i);
bool	is_simple_operator(t_mini_data *data, int *i);
bool	is_a_redirection(t_mini_data *data, int *i);
bool	is_a_aggregate_redirection(t_mini_data *data, int *i);
bool	is_a_simple_redirection(t_mini_data *data, int *i);
bool	process_token(t_mini_data *data, int *i, bool (*cnd)(char), \
						t_tokentype type);
bool	proc_tok_off_2(t_mini_data *data, int *i, bool (*cnd)(char), int type);
bool	proc_token_off_1(t_mini_data *data, int *i, bool (*cnd)(char), int typ);
bool	add_block_dbl_paren(t_mini_data *data, int *i, char *delim, int t_type);
bool	add_tokenblock(t_mini_data *data, int *i, char delim, int t_type);
bool	add_parenthesisblock(t_mini_data *data, int *i, char delim, int t_type);
bool	is_a_block(t_mini_data *data, int *i);
bool	is_a_hist_expansion(t_mini_data *data, int *i);
bool	is_a_dollar_exp(t_mini_data *data, int *i);
bool	is_a_flag(t_mini_data *data, int *i);
bool	is_a_string_thing(t_mini_data *data, int *i);
bool	is_a_redirection(t_mini_data *data, int *i);
bool	is_a_globbing(t_mini_data *data, const char *tmp, int *start);
bool	is_redirection_token(t_tokentype tokentype);
bool	is_heredoc_token(t_tokentype tokentype);

#  ifdef __cplusplus

}
#  endif

# endif  // SCANNER_H_
