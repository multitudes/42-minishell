/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:47:11 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/12 19:03:48 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
peek wil look ahead to see if my string is beginning with sequence of chars
- input is the string to check
- identifier is the string to check for
- need_delim is true if the identifier must be followed by a space to be valid
ex '||' works without spaces at the end but 'echo' is valid with space only 
or with 
one of the metacharacters : ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
peek is case insensitive!
*/
bool	peek(const char *input, const char *identifier, bool need_delim)
{
	int	i;
	int	n;

	i = 0;
	n = ft_strlen(identifier);
	while (i < n && cmp_char_case_insensitive(input[i], identifier[i]))
		i++;
	if (i == n && ((need_delim && is_delimiter(input[i])) || !need_delim))
		return (true);
	else
		return (false);
}

/*
 mh.. just checked again in the bash and maybe the function below is not 
 necessary? TODO
 bash lets me create a var while and print it with $while... 	
 so not for identifier.  they cannot be used for commands...!
 IF, THEN, ELSE, ELIF, FI, DO, DONE, WHILE, UNTIL, FOR, CASE, ESAC, SELECT,
  FUNCTION,
*/
bool	is_reserved1(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "while", true))
		add_token(data, start, "while", WHILE);
	else if (peek(identifier, "until", true))
		add_token(data, start, "until", UNTIL);
	else if (peek(identifier, "for", true))
		add_token(data, start, "for", FOR);
	else if (peek(identifier, "case", true))
		add_token(data, start, "case", CASE);
	else if (peek(identifier, "esac", true))
		add_token(data, start, "esac", ESAC);
	else if (peek(identifier, "select", true))
		add_token(data, start, "select", SELECT);
	else if (peek(identifier, "function", true))
		add_token(data, start, "function", FUNCTION);
	else if (peek(identifier, "in", true))
		add_token(data, start, "in", IN);
	else
		return (false);
	return (true);
}

bool	is_reserved2(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "if", true))
		add_token(data, start, "if", IF);
	else if (peek(identifier, "then", true))
		add_token(data, start, "then", THEN);
	else if (peek(identifier, "else", true))
		add_token(data, start, "else", ELSE);
	else if (peek(identifier, "elif", true))
		add_token(data, start, "elif", ELIF);
	else if (peek(identifier, "fi", true))
		add_token(data, start, "fi", FI);
	else if (peek(identifier, "do", true))
		add_token(data, start, "do", DO);
	else if (peek(identifier, "done", true))
		add_token(data, start, "done", DONE);
	else
		return (false);
	return (true);
}

bool	is_reserved(t_mini_data *data, char *identifier, int *start)
{
	if (is_reserved1(data, identifier, start))
		return (true);
	else if (is_reserved2(data, identifier, start))
		return (true);
	else
		return (false);
}

/*
bash builtins which we do not implement are in this list (taken from the 
bash manual but enhanced by copilot)
"alias bg bind break builtin caller command compgen 
. : complete continue declare dirs disown enable eval exec fc fg getopts hash 
help history jobs kill let local logout mapfile popd printf pushd read 
readonly return set shift shopt source suspend test times trap type 
typeset ulimit umask unalias wait readarray"
*/
bool	not_implemented_builtin(const char *id)
{
	if (!strncicmp(id, "bg", 3) || !strncicmp(id, "fc", 2) || !strncicmp(id, \
	":", 2) || !strncicmp(id, "bind", 5) || !strncicmp(id, "break", 6) || \
	!strncicmp(id, "builtin", 8) || !strncicmp(id, "caller", 6) || \
	!strncicmp(id, "command", 7) || !strncicmp(id, "compgen", 7) || \
	!strncicmp(id, ".", 2) || !strncicmp(id, "complete", 9) || !strncicmp(id, \
	"continue", 8) || !strncicmp(id, "declare", 7) || !strncicmp(id, "dirs", \
	4) || !strncicmp(id, "disown", 6) || !strncicmp(id, "enable", 6) || \
	!strncicmp(id, "eval", 4) || !strncicmp(id, "exec", 4) || !strncicmp(id, \
	"alias", 6) || !strncicmp(id, "fg", 2) || !strncicmp(id, "getopts", 7) || \
	!strncicmp(id, "hash", 4) || !strncicmp(id, "help", 4) || !strncicmp(id, \
	"history", 7) || !strncicmp(id, "jobs", 4) || !strncicmp(id, "kill", 4) || \
	!strncicmp(id, "let", 3) || !strncicmp(id, "local", 5) || !strncicmp(id, \
	"logout", 6) || !strncicmp(id, "mapfile", 7) || !strncicmp(id, "popd", 4) \
	|| !strncicmp(id, "printf", 6) || !strncicmp(id, "pushd", 5) || \
	!strncicmp(id, "read", 4) || !strncicmp(id, "readonly", 8) || \
	!strncicmp(id, "return", 6) || !strncicmp(id, "set", 3) || \
	!strncicmp(id, "shift", 5) || !strncicmp(id, "shopt", 5) || \
	!strncicmp(id, "source", 6) || !strncicmp(id, "suspend", 7) || \
	!strncicmp(id, "test", 4) || !strncicmp(id, "times", 5) || \
	!strncicmp(id, "trap", 4) || !strncicmp(id, "type", 4) || \
	!strncicmp(id, "typeset", 7) || !strncicmp(id, "ulimit", 6) || \
	!strncicmp(id, "umask", 5) || !strncicmp(id, "unalias", 7) || \
	!strncicmp(id, "wait", 4) || !strncicmp(id, "readarray", 9))
		return (true);
	return (false);
}

/*
is_builtin checks if the identifier is a builtin command
*/
bool	is_builtin(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "echo", true))
		add_token(data, start, "echo", BUILTIN);
	else if (peek(identifier, "cd", true))
		add_token(data, start, "cd", BUILTIN);
	else if (peek(identifier, "export", true))
		add_token(data, start, "export", BUILTIN);
	else if (peek(identifier, "unset", true))
		add_token(data, start, "unset", BUILTIN);
	else if (peek(identifier, "env", true))
		add_token(data, start, "env", BUILTIN);
	else if (peek(identifier, "exit", true))
		add_token(data, start, "exit", BUILTIN);
	else if (peek(identifier, "pwd", true))
		add_token(data, start, "pwd", BUILTIN);
	else if (not_implemented_builtin(identifier))
		add_token(data, start, identifier, BUILTIN);
	else
		return (false);
	return (true);
}

/*
creates a simple t_list node - the token is in the content of the node
in form of a string that will need to be freed
*/
t_list	*create_token(t_tokentype type, const char *lexeme, int *i)
{
	t_token	*token;
	t_list	*new_node;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->lexeme = ft_strdup(lexeme);
	token->start = *i;
	*i = *i + ft_strlen(lexeme);
	debug("token created %p -%s-", token, token->lexeme);
	new_node = ft_lstnew(token);
	if (new_node == NULL)
	{
		free_token(token);
		return (NULL);
	}
	return (new_node);
}

/*
I have a linked lists of nodes. each node's content is a token
I need to pass this function to my ft_lstclear to clear the list contents
and because the content is a token which 
-has a lexeme which needs to be free
-and then the token itself needs to be freed
*/
void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	free(token->lexeme);
	free(token);
}

/*
io numbers are the one preceding a < or > in a redirection
cannot start with a 0
*/
bool	is_io_number(const char *str)
{
	if (*str == '0' && is_digit(*(str + 1)))
		return (false);
	while (is_digit(*str))
		str++;
	if (*str == '\0')
		return (true);
	return (false);
}

/*
including numbers preceded by a - or + 
and with and without a dot
*/
bool	str_is_number(const char *str)
{
	int	dot_seen;

	dot_seen = 0;
	if ((*str == '-' || *str == '+') && is_digit(*(str + 1)))
		str++;
	while (*str)
	{
		if (*str == '.')
		{
			if (dot_seen)
				return (false);
			dot_seen = 1;
		}
		else if (!is_digit(*str))
			return (false);
		str++;
	}
	return (true);
}

/*
this function checks if the tmp string is NULL and also if 
the malloc in create token fails and updates the error flag in the 
data struct so the loop will stop and free the data
it still returns true because when a token subfunction returns true
it means the token has been recognized and the scanner can move on
otherwise it would keep on looking for the token
*/
bool	add_token(t_mini_data *data, int *i, const char *tmp, int type)
{
	t_list	*token;

	if (!tmp)
		scanner_error(data, "error: malloc tmp failed");
	else
	{
		token = create_token(type, tmp, i);
		if (token)
			ft_lstadd_back(&data->token_list, token);
		else
			scanner_error(data, "error: malloc token failed");
	}
	return (true);
}

bool	is_a_control_operator(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "||", false))
		add_token(data, i, "||", OR_IF);
	else if (peek(data->input + *i, "&&", false))
		add_token(data, i, "&&", AND_IF);
	else if (peek(data->input + *i, "|&", false))
		add_token(data, i, "|&", PIPE_AND);
	else if (peek(data->input + *i, ";&", false))
		add_token(data, i, ";&", SEMI_AND);
	else if (peek(data->input + *i, "|", false))
		add_token(data, i, "|", PIPE);
	else
		return (false);
	return (true);
}

bool	is_a_math_op(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "--", false))
		add_token(data, i, "--", MINUS_MINUS);
	else if (peek(data->input + *i, "++", false))
		add_token(data, i, "++", PLUS_PLUS);
	else if (peek(data->input + *i, "-=", false))
		add_token(data, i, "-=", MINUS_EQUAL);
	else if (peek(data->input + *i, "+=", false))
		add_token(data, i, "+=", PLUS_EQUAL);
	else if (peek(data->input + *i, "/=", false))
		add_token(data, i, "/=", SLASH_EQUAL);
	else if (peek(data->input + *i, "*=", false))
		add_token(data, i, "*=", STAR_EQUAL);
	else
		return (false);
	return (true);
}

/*
why returning true. I want to tell the scanner that the 
token was successfully recognized so that it stops looking
for other tokens and go back to the while loop with the data error 
in the data struct so it will stop and free the data
*/
bool	scanner_error(t_mini_data *data, char *err_str)
{
	debug("error: %s\n", err_str);
	data->scanner_err_str = err_str;
	data->scanner_error = 1;
	return (true);
}

/*
just for style, it removes ugly pointer arythmetics 
from my code
*/
void	advance(int *i)
{
	(*i)++;
}

/*
condition is a pointer to a function that will be used to check the
character in the wjile loop. I might need is_digit for numbers or
is_alnum for identifiers etc
Thgis works for easy tokens.
*/
bool	process_token(t_mini_data *data, int *i, bool (*cnd)(char), int type)
{
	int		start;
	char	*tmp;

	start = *i;
	while (cnd(data->input[*i]))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start);
	add_token(data, &start, tmp, type);
	free(tmp);
	*i = start;
	return (true);
}

/*
condition is a pointer to a function that will be used to check the
character in the wjile loop. I might need is_digit for numbers or
is_alnum for identifiers etc
Thgis works for easy tokens.
*/
bool	proc_token_off_1(t_mini_data *data, int *i, bool (*cnd)(char), int type)
{
	int		start;
	char	*tmp;

	start = (*i)++;
	while (cnd(data->input[*i]))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start);
	add_token(data, &start, tmp, type);
	free(tmp);
	*i = start;
	return (true);
}

/*
condition is a pointer to a function that will be used to check the
character in the while loop. I might need is_digit for numbers or
is_alnum for identifiers etc
This is offset of 2 lets say I look for !- followed by a number of digits
*/
bool	proc_tok_off_2(t_mini_data *data, int *i, bool (*cnd)(char), int type)
{
	int		start;
	char	*tmp;

	start = (*i);
	while (cnd(data->input[*i] + 2))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start + 3);
	add_token(data, &start, tmp, type);
	free(tmp);
	*i = start;
	return (true);
}

/*
passing the negated condition !is_delimiter as a func pointer 
is not possible unless I create a new function  
*/
bool	is_not_delimiter(char c)
{
	return (!is_delimiter(c));
}

/*
History expansion - (not implemented)!
!!: Re-run the previous command. 
This is useful if you forgot to use sudo for a command that 
requires it. You can simply type sudo !! to re-run the previous 
command with sudo.

!n: Re-run the nth command in your history. 
For example, !100 would re-run the 100th command.

!-n: Re-run the command n lines back. 
For example, !-2 would re-run the second most recent command.

!string: Re-run the most recent command that 
starts with string. For example, !ls would re-run the most recent command 
that starts with ls.

!?string?: Re-run the most recent command 
that contains string anywhere. For example, !?txt? would re-run the most 
recent command that includes txt.
BANG_BANG,BANG_DIGIT, BANG_HYPHEN_DIGIT, BANG_ALPHA, BANG_QUESTION_ALPHA
*/
bool	is_a_hist_expansion(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "!!", false))
		return (process_token(data, i, is_not_delimiter, BANG_BANG));
	else if (peek(data->input + *i, "!", false) && is_digit(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_digit, BANG_DIGIT));
	else if (peek(data->input + *i, "!-", false) && is_digit(*(data->input \
	+ *i + 2)))
		return (proc_tok_off_2(data, i, is_digit, BANG_HYPHEN_DIGIT));
	else if (peek(data->input + *i, "!", false) && is_alpha(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_alpha, BANG_ALPHA));
	else if (peek(data->input + *i, "!?", false))
		return (process_token(data, i, is_not_delimiter, BANG_QUESTION_ALPHA));
	else if (peek(data->input + *i, "!#", false))
		return (add_token(data, i, "!#", BANG_HASH));
	return (false);
}

/*
I define a block as the text between two delimiters like {}
or "" '' or `` or () etc
anything until I get the closing delimiter I specify in delim...
*/
bool	add_tokenblock(t_mini_data *data, int *i, char delim, int t_type)
{
	int		start;
	char	*tmp;

	start = (*i)++;
	while (data->input[*i] && data->input[*i] != delim)
		advance(i);
	if (data->input[*i] == '\0')
		return (scanner_error(data, "error: unclosed expression"));
	tmp = ft_substr(data->input, start, *i - start + 1);
	add_token(data, &start, tmp, t_type);
	*i = start;
	free(tmp);
	return (true);
}

/*
arithmetic expansion is $(()) and I need to find the closing ))
*/
bool	add_block_dbl_paren(t_mini_data *data, int *i, char *delim, int t_type)
{
	int		start;
	char	*tmp;

	start = (*i)++;
	while (peek(data->input + *i, delim, false) == false)
		advance(i);
	if (*(data->input + *i + 1) == '\0')
		return (scanner_error(data, "error: unclosed expansion"));
	tmp = ft_substr(data->input, start, *i - start + 2);
	*i = start;
	add_token(data, i, tmp, t_type);
	free(tmp);
	return (true);
}

bool	add_here_and_delim(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	add_token(data, i, "<<", DLESS);
	while (data->input[*i] && is_space(data->input[*i]))
		advance(i);
	if (data->input[*i] == '\0' || is_delimiter(data->input[*i]))
		return (scanner_error(data, "error: missing here-delim"));
	start = *i;
	while ((data->input + *i) && !is_delimiter(data->input[*i]))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start);
	*i = start;
	add_token(data, i, tmp, DLESS_DELIM);
	free(tmp);
	return (true);
}

bool	is_simple_dollar_exp(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "$?", false))
		return (add_token(data, i, "$?", DOLLAR_QUESTION));
	else if (peek(data->input + *i, "$$", false))
		return (add_token(data, i, "$$", DOLLAR_DOLLAR));
	else if (peek(data->input + *i, "$*", false))
		return (add_token(data, i, "$*", DOLLAR_STAR));
	else if (peek(data->input + *i, "$@", false))
		return (add_token(data, i, "$@", DOLLAR_AT));
	else if (peek(data->input + *i, "$#", false))
		return (add_token(data, i, "$#", DOLLAR_HASH));
	else if (peek(data->input + *i, "$!", false))
		return (add_token(data, i, "$!", DOLLAR_BANG));
	else if (peek(data->input + *i, "$-", false))
		return (add_token(data, i, "$-", DOLLAR_HYPHEN));
	else
		return (false);
}

bool	is_complex_dollar_exp(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "$((", false))
		return (add_block_dbl_paren(data, i, "))", EXPR_EXPANSION));
	else if (peek(data->input + *i, "$", false) && \
	is_digit(*(data->input + *i + 1)))
		return (proc_token_off_1(data, i, is_digit, \
		DOLLAR_DIGIT));
	else if (peek(data->input + *i, "${", false))
		return (add_tokenblock(data, i, '}', VAR_EXPANSION));
	else if (peek(data->input + *i, "$", false) && is_alnum(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_alnum, VAR_EXPANSION));
	else if (peek(data->input + *i, "$(", false))
		return (add_tokenblock(data, i, ')', COM_EXPANSION));
	else
		return (false);
}

/*
    DOLLAR_QUESTION, // '$?'  The special parameter ‘?’ is used to get the 
	exit status of the last command.
    DOLLAR_DOLLAR, // '$$' ‘$’ is used to get the process ID of the shell.
    DOLLAR_STAR, // '$*' ‘*’ is used to get all the positional parameters.
    DOLLAR_AT, // '$@'  ‘@’ is used to get all the positional parameters, 
	except for the zeroth positional parameter.
    DOLLAR_HASH, // '$#'  ‘#’ is used to get the number of positional 
	parameters.
    DOLLAR_BANG, // '$!'  ‘!’ is used to get the process ID of the last 
	background command.
	DOLLAR_HYPHEN, // '$-' used to get the current options set for the shell.	 
	DOLLAR_DIGIT, // '$0' ‘0’ is used to get the name of the shell or script.
	Parameter names in bash can only contain alphanumeric 
	characters or underscores, and must start with a letter or underscore.
*/
bool	is_a_dollar_exp(t_mini_data *data, int *i)
{
	if (is_simple_dollar_exp(data, i))
		return (true);
	else if (is_complex_dollar_exp(data, i))
		return (true);
	else
		return (false);
}

bool	is_a_simple_redirection(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, ">|", false))
		return (add_token(data, i, ">|", CLOBBER));
	else if (peek(data->input + *i, ">>&", false))
		return (add_token(data, i, ">>&", REDIRECT_BOTH_APP));
	else if (peek(data->input + *i, "<<-", false))
		return (add_token(data, i, "<<-", DLESSDASH));
	else if (peek(data->input + *i, "&>>", false))
		return (add_token(data, i, "&>>", REDIRECT_OUT_APP));
	else if (peek(data->input + *i, ">&>", false))
		return (add_token(data, i, ">&>", GREATER_AND_GREATER));
	else if (peek(data->input + *i, "&>", false))
		return (add_token(data, i, "&>", REDIRECT_BOTH));
	else if (peek(data->input + *i, "<&", false))
		return (add_token(data, i, "<&", LESSAND));
	else if (peek(data->input + *i, ">&", false))
		return (add_token(data, i, ">&", GREATAND));
	else if (peek(data->input + *i, "<>", false))
		return (add_token(data, i, "<>", LESSGREAT));
	else if (peek(data->input + *i, ">>", false))
		return (add_token(data, i, ">>", DGREAT));
	else
		return (false);
	return (true);
}

bool	is_a_aggregate_redirection(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "<<", false))
		return (add_here_and_delim(data, i));
	else if (peek(data->input + *i, ">=", false))
		return (add_token(data, i, ">=", GREATER_EQUAL));
	else if (peek(data->input + *i, "<=", false))
		return (add_token(data, i, "<=", LESS_EQUAL));
	else if (peek(data->input + *i, "!=", false))
		return (add_token(data, i, "!=", BANG_EQUAL));
	else if (peek(data->input + *i, ">", false))
		return (add_token(data, i, ">", REDIRECT_OUT));
	else if (peek(data->input + *i, "<", false))
		return (add_token(data, i, "<", REDIRECT_IN));
	else if (peek(data->input + *i, "==", false))
		return (add_token(data, i, "==", EQUAL_EQUAL));
	else if (peek(data->input + *i, ",", false))
		return (add_token(data, i, ",", COMMA));
	else if (peek(data->input + *i, "-", true))
		return (add_token(data, i, "-", MINUS));
	else if (peek(data->input + *i, "+", false))
		return (add_token(data, i, "+", PLUS));
	else if (peek(data->input + *i, "*", false))
		return (add_token(data, i, "*", STAR));
	else
		return (false);
}

/****************************************/
/* redirections							*/
/****************************************/
bool	is_a_redirection(t_mini_data *data, int *i)
{
	if (is_a_simple_redirection(data, i))
		return (true);
	else if (is_a_aggregate_redirection(data, i))
		return (true);
	else
		return (false);
}

/*
*/
bool	is_a_block(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "(", false))
		return (add_tokenblock(data, i, ')', EXPRESSION));
	else if (peek(data->input + *i, "\'", false))
		return (add_tokenblock(data, i, '\'', S_QUOTED_STRING));
	else if (peek(data->input + *i, "\"", false))
		return (add_tokenblock(data, i, '\"', QUOTED_STRING));
	else if (peek(data->input + *i, "`", false))
		return (add_tokenblock(data, i, '`', COM_EXPANSION));
	return (false);
}

/*
create a lexeme for flag in this conf -[a-zA-Z]
else if (peek((data->input + i), "-", false) && is_alpha(data->input[i + 1]))
*/
bool	is_a_flag(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	if (peek(data->input + *i, "-", false) && is_alpha(data->input[*i + 1]))
	{
		start = (*i)++;
		while (data->input[*i] && is_alpha(data->input[*i]))
			advance(i);
		tmp = ft_substr(data->input, start, *i - start);
		add_token(data, &start, tmp, FLAGS);
		*i = start;
		free(tmp);
		return (true);
	}
	return (false);
}

bool	is_simple_operator(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "=", false))
		return (add_token(data, i, "=", EQUAL));
	else if (peek(data->input + *i, "&", false))
		return (add_token(data, i, "&", AND_TOK));
	else if (peek(data->input + *i, "^", false))
		return (add_token(data, i, "^", CARET));
	else if (peek(data->input + *i, "%", false))
		return (add_token(data, i, "%", PERCENT));
	else if (peek(data->input + *i, "~", false))
		return (add_token(data, i, "~", TILDE));
	else if (peek(data->input + *i, "$", false))
		return (add_token(data, i, "$", DOLLAR));
	else
		return (false);
}

bool	is_some_semicolons(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, ";;&", false))
		return (add_token(data, i, ";;&", DSEMI_AND));
	else if (peek(data->input + *i, ";;", false))
		return (add_token(data, i, ";;", DSEMI));
	else if (peek(data->input + *i, ";", false))
		return (add_token(data, i, ";", SEMI));
	else if (peek(data->input + *i, "!", true))
		return (add_token(data, i, "!", BANG));
	else
		return (false);
}

bool	is_delimited_string(char c)
{
	return (ft_isprint(c) && !filename_delimiter(c));
}

bool	is_a_string_thing(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	start = *i;
	while (is_delimited_string(data->input[*i]))
		advance(i);
	if (*i == start)
		return (false);
	tmp = ft_substr(data->input, start, *i - start);
	if ((data->input[*i] == '<' || data->input[*i] == '>') \
	&& is_io_number(tmp))
		add_token(data, &start, tmp, IO_NUMBER);
	else if (!is_reserved(data, tmp, &start) && !is_builtin(data, \
	tmp, &start) && !is_true_false(data, tmp, &start) && \
	!is_a_pathname_or_num(data, tmp, &start))
		add_token(data, &start, tmp, WORD);
	free(tmp);
	return (true);
}

/*

*/
bool	got_tokens(t_mini_data *data, int *i)
{
	if (is_a_control_operator(data, i) || is_a_math_op(data, i) || \
	is_a_hist_expansion(data, i) || is_a_dollar_exp(data, i) || \
	is_some_semicolons(data, i) || is_a_redirection(data, i) || \
	is_a_redirection(data, i) || is_simple_operator(data, i) || \
	is_a_block(data, i) || is_a_flag(data, i) || is_a_string_thing(data, i))
		return (true);
	else
		return (false);
}

/*
scanning function
Returns a token list which is a linked list of t_list type
note: we dont need to action some of them but fir completeness I added 
as much as I could. for instance we do not action the last & on a command
or redirections like 
*/
t_list	*tokenizer(t_mini_data *data)
{
	int	i;

	i = 0;
	data->exit_status = 0;
	data->token_list = NULL;
	while (i < (int)ft_strlen(data->input) && data->exit_status == 0)
	{
		if (peek(data->input + i, "#", false))
			break ;
		else if (got_tokens(data, &i))
			continue ;
		else if (!is_space(data->input[i]))
			scanner_error(data, "error: unrecognized token");
		else
			i++;
	}
	if (data->exit_status == 0)
		return (data->token_list);
	ft_lstclear(&data->token_list, free_token);
	return (NULL);
}
