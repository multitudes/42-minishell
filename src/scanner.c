/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:47:11 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/12 12:38:50 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 ``\t''``\n''``\v''``\f''``\r''`` -> is space 
 overkill I think. From the bash manual they talk only tab and space and newline which
 in my case it is impossible because caught by the readline func
*/
bool is_space(const char c)
{
	if (c == 0)
		return (false);
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

/*
string n compare but case insensitive
*/
int strncicmp(char const *a, char const *b, int n)
{
	if (n == 0)
		return 0;
	if (a == NULL || b == NULL || n < 0)
		return -1;
	while (*a && *b && n--)
	{
		int d = ft_tolower(*a) - ft_tolower(*b);
		if (d != 0)
			return d;
		a++, b++;
	}
	return *a - *b;
}

/* Returns 'true' if the character is a delimiter. Because an identifier
can be unofficially almost anything, the delimiter is a character that
should not be part of an identifier. Recommended char for identifiers
are underscore and alphanumeric characters.
but from the bash manual:
A metacharacter is a space, tab, newline, 
or one of the following characters: 
‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
I use this function to understand where to break the string
into tokens
*/
bool is_delimiter(char ch)
{
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '|' || \
	ch == '&' || ch == ';' || ch == '(' || ch == ')' || \
	ch == '>' || ch == '<' || ch == '\0')
		return (true);
    return (false);
}

/*
 ... touch w$orld is creating w because dereferencing 
the $ and the rest is nulll...
otherwise 
touch w-+*}\,[]=w{$HOME
becomes touch: w-+*},[]=w{/Users/laurentb: Not a directory
touch w-+*}\,[]=w{&orld
bash: orld: command not found
touch w-+*}\,[]=w{|orld
bash: orld: command not found
[1]+  Done                    touch w-+*}\,[]=w{
The command touch w-+*}\,[]=w{|orld is being interpreted by the 
shell (bash in this case) before it's passed to the touch command.
In this case, touch w-+*}\,[]=w{ is being 
run as one command, and orld is being interpreted as a 
separate command.
The output of touch w-+*}\,[]=w{ (which is nothing, 
because touch doesn't produce any output) is being passed as input to orld.
However, orld is not a valid command, 
so bash gives an error message: bash: orld: command not found.
The [1]+ Done touch w-+*}\,[]=w{ message is from bash's job 
control system. It's telling you that the touch w-+*}\,[]=w{ command has finished running.


touch w-+*},[]=w{\o!rld bash: !rld: event not found

The ! character has a special meaning in many Unix and Linux shells, 
including bash. It is used for history expansion, 
allowing you to re-run previous commands.
In your command touch w-+*}\,[]=w{\o!rld, the ! character 
is causing bash to attempt to perform history expansion

ok these char cannot be in a file name.. 
officially it is recommended to use only the 65 characters [-._a-zA-Z0-9]
for files and directories ...
*/
bool filename_delimiter(const char ch)
{
    if (is_space(ch) || ch == ';' || ch == '>' || 
        ch == '<' || ch == '(' || ch == ')' || 
        ch == '|' || ch == '&' || ch == '$' || ch == '`' ||
        ch == '"' || ch == '\''  || ch == '!' ||
        ch == '\0')
		return (true);
    return (false);
}

/*
Returns 'true' if the character is a digit
*/
bool is_digit(char c) {
      return c >= '0' && c <= '9';
}

/*
Returns 'true' if the character is char a letter
*/
bool is_alpha(char c) 
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/*
part of the check for identifiers
btw This 65-character set, [-._a-zA-Z0-9], 
is referred to in SUSv3 as the portable filename character set.
*/
bool is_alnum(char c) 
{
	return (is_alpha(c) || is_digit(c) || c == '_' || c == '-' || c == '.');
}

/*
either is alnum including the _ or is a . / ~ -
*/
bool is_pathname(char c) 
{
	return is_alnum(c) || c == '.' || c == '-' || c == '/' || c == '~' ;
}


/*
compare two chars case insensitive - used in peek!
*/

bool cmp_char_case_insensitive(char a, char b)
{
	if (is_alpha(a) && is_alpha(b))
		return (ft_tolower(a) == ft_tolower(b));
	else
		return (a == b);
}
/*
peek wil look ahead to see if my string is beginning with sequence of chars
- input is the string to check
- identifier is the string to check for
- need_delim is true if the identifier must be followed by a space to be valid
ex '||' works without spaces at the end but 'echo' is valid with space only or with 
one of the metacharacters : ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
peek is case insensitive!
*/
bool	peek(const char *input, const char *identifier, bool need_delim)
{
	int i;
	int n;

	i = 0;
	n = ft_strlen(identifier);
	while (i < n && cmp_char_case_insensitive(input[i], identifier[i])) // add compare case insensitive
		i++;
	if (i == n && ((need_delim && is_delimiter(input[i])) || !need_delim)) 
		return (true);
	else
		return (false);
}

/*
 mh.. just checked again in the bash and maybe the function below is not necessary? TODO
 bash lets me create a var while and print it with $while... 	
 so not for identifier.  they cannot be used for commands...!
 IF, THEN, ELSE, ELIF, FI, DO, DONE, WHILE, UNTIL, FOR, CASE, ESAC, SELECT, FUNCTION,
*/
bool is_reserved(t_mini_data *data, char *identifier, int *start)
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
	else if (peek(identifier, "while", true))
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
	return true;
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
	if ((strncicmp(id, "alias", 6) == 0) || strncicmp(id, "bg", 3) == 0 || \
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
		return (true);
	return (false);
}
/*
is_builtin checks if the identifier is a builtin command
*/
bool is_builtin(t_mini_data *data, char *identifier, int *start)
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
		return false;
	return true;
}

/*
 * is_true_false checks if the identifier is a boolean value
 * like in bash true and false are not builtins but reserved words
 */
bool	is_true_false(t_mini_data *data, char *str, int *start)
{
	if (peek(str, "true", true))
		add_token(data, start, "true", TRUETOK);
	else if (peek(str, "false", true))
		add_token(data, start, "false", FALSETOK);
	else
		return false;
	return true;
}


void print_token_list(t_list *token_list)
{
	t_list *current;
	t_token *token;
	
	current = token_list;
	while (current != NULL)
	{
		token = (t_token *)(current)->content;
		debug("token type :%u, lexeme: %s position %d", token->type, token->lexeme, token->start);
		current = current->next;
	}
	return ;
}

/*
creates a simple t_list node - the token is in the content of the node
in form of a string that will need to be freed
*/
t_list *create_token(t_tokentype type, const char *lexeme, int *i)
{
	t_token *token;
	t_list *new_node;

	token = malloc(sizeof(t_token));
	if (token == NULL)
	{
		perror("malloc token");
		return (NULL);
	}
	token->type = type;
	token->lexeme = ft_strdup(lexeme);
	token->start = *i;
	*i = *i + ft_strlen(lexeme);
	debug("token created %p -%s-", token, token->lexeme);
	new_node = ft_lstnew(token);
	if (new_node == NULL)
	{
		perror("malloc new_node");
		free(token->lexeme);
		free(token);
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
	t_token *token;
	
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
bool	str_is_number(const char *identifier)
{
    int dot_seen = 0;

    if ((*identifier == '-' || *identifier == '+') && is_digit(*(identifier + 1)))
        identifier++;

    while (*identifier)
    {
        if (*identifier == '.')
        {
            if (dot_seen) // if we've already seen a dot, it's not a valid number
                return (false);
            dot_seen = 1;
        }
        else if (!is_digit(*identifier))
            return (false);
        identifier++;
    }

    return (true);
}

/*
*/
bool	str_is_alphanum(const char *str)
{
	while (*str)
		if (!is_alnum(*(str++)))
			return (false);
	return (true);
}

/*
printable chars include a space. This is for the identifiers
*/
int	isprint_no_space(const char *str)
{
	while (*(str))
	{
		if (is_space(*str) || !isprint(*str))
			return (0);
		str++;
	}
	return (1);
}

/*
this function checks if the tmp string is NULL and also if 
the malloc in create token fails and updates the error flag in the 
data struct so the loop will stop and free the data
it still returns true because when a token subfunction returns true
it means the token has been recognized and the scanner can move on
otherwise it would keep on looking for the token
*/
bool	add_token(t_mini_data *data, int *i, char *tmp, enum e_tokentype type)
{
	if (!tmp)
		scanner_error(data, "error: malloc tmp failed");
	else 
	{
		t_list *token = create_token(type, tmp, i);
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

*/
void advance(int *i)
{
	(*i)++;
}

bool process_token(t_mini_data *data, int *i, bool (*condition)(char), int token_type)
{
    int start = *i;
    while (condition(data->input[*i]))
        advance(i);
    char *tmp = ft_substr(data->input, start, *i - start);
    add_token(data, &start, tmp, token_type);
    free(tmp);
    *i = start;
    return (true);
}

bool is_not_delimiter(char c) {
    return !is_delimiter(c);
}

/*
	History expansion - (not implemented)
	!!: Re-run the previous command. 
	This is useful if you forgot to use sudo for a command that 
	requires it. You can simply type sudo !! to re-run the previous command with sudo.

	!n: Re-run the nth command in your history. 
	For example, !100 would re-run the 100th command.

	!-n: Re-run the command n lines back. 
	For example, !-2 would re-run the second most recent command.

	!string: Re-run the most recent command that 
	starts with string. For example, !ls would re-run the most recent command that starts with ls.

	!?string?: Re-run the most recent command 
	that contains string anywhere. For example, !?txt? would re-run the most recent command that includes txt.
	BANG_BANG,BANG_DIGIT, BANG_HYPHEN_DIGIT, BANG_ALPHA, BANG_QUESTION_ALPHA
	*/
bool is_a_hist_expansion(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "!!", false)) 
		return (process_token(data, i, is_not_delimiter, BANG_BANG));
	else if (peek(data->input + *i, "!", false) && is_digit(*(data->input + *i + 1)))
	{
		int start = (*i)++;
		while (is_digit(data->input[*i]))
			advance(i);
		char *tmp = ft_substr(data->input, start, *i - start);
		add_token(data, &start, tmp, BANG_DIGIT);
		free(tmp);
		*i = start;
		return (true);
	}	
	else if (peek(data->input + *i, "!-", false) && is_digit(*(data->input + *i + 2)))
	{
		int start = *i;
		while (is_digit(*(data->input + *i + 2)))
			advance(i);
		char *tmp = ft_substr(data->input, start, *i - start + 2);
		add_token(data, &start, tmp, BANG_HYPHEN_DIGIT);
		free(tmp);
		*i = start;
		return (true);
	}
	else if (peek(data->input + *i, "!", false) && is_alpha(*(data->input + *i + 1)))
	{
		int start = (*i)++;
		while (is_alpha(data->input[*i]))
			advance(i);
		char *tmp = ft_substr(data->input, start, *i - start);
		add_token(data, &start, tmp, BANG_ALPHA);
		*i = start;
		free(tmp);
		return (true);
	}
	if (peek(data->input + *i, "!?", false)) 
		return (process_token(data, i, is_not_delimiter, BANG_QUESTION_ALPHA));
	else if (peek(data->input + *i, "!#", false))
		return (add_token(data, i, "!#", BANG_HASH));
	return (false);
}

/*
    DOLLAR_QUESTION, // '$?'  The special parameter ‘?’ is used to get the exit status of the last command.
    DOLLAR_DOLLAR, // '$$' ‘$’ is used to get the process ID of the shell.
    DOLLAR_STAR, // '$*' ‘*’ is used to get all the positional parameters.
    DOLLAR_AT, // '$@'  ‘@’ is used to get all the positional parameters, except for the zeroth positional parameter.
    DOLLAR_HASH, // '$#'  ‘#’ is used to get the number of positional parameters.
    DOLLAR_BANG, // '$!'  ‘!’ is used to get the process ID of the last background command.
	DOLLAR_HYPHEN, // '$-' used to get the current options set for the shell.	 
	DOLLAR_DIGIT, // '$0' ‘0’ is used to get the name of the shell or script.
*/
bool is_a_dollar_exp(t_mini_data *data, int *i)
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

	// command expansion $(command) or `command`
	else if (peek(data->input + *i, "$((", false))
	{
		debug("found $((");
		int start = (*i)++;
		while (peek(data->input + *i, "))", false) == false)
			advance(i);
		if (*(data->input + *i + 1) == '\0')
			return (scanner_error(data, "error: unclosed expansion"));
		char *tmp = ft_substr(data->input, start, *i - start + 2);
		if (!tmp)
			return (scanner_error(data, "error: malloc tmp failed"));
		*i = start;
		add_token(data, i, tmp, EXPR_EXPANSION);
		free(tmp);
		return (true);
	}
	// $1 to $9
	else if (peek(data->input + *i, "$", false) && is_digit(*(data->input + *i + 1)))
	{
		int start = (*i)++;
		while (is_digit(data->input[*i]))
			advance(i);
		char *tmp = ft_substr(data->input, start, *i - start);
		*i = start;
		add_token(data, i, tmp, DOLLAR_DIGIT);
		free(tmp);
		return (true);
	}
	// expansion ${parameter} or $parameter or $(command) or $((arythm expression))
	else if (peek(data->input + *i, "${", false))
	{
		int start = (*i)++;
		while (data->input[*i] != '}')
			advance(i);
		if (data->input[*i] == '\0')
			return (scanner_error(data, "error: unclosed expansion"));
		char *tmp = ft_substr(data->input, start, *i - start + 1);
		*i = start;
		add_token(data, i, tmp, VAR_EXPANSION);
		free(tmp);
		return (true);
	}
	// Parameter names in bash can only contain alphanumeric 
	// characters or underscores, and must start with a letter or underscore.
	else if (peek(data->input + *i, "$", false) && is_alnum(*(data->input + *i + 1)))
	{
		int start = (*i)++;
		while (is_alnum(data->input[*i]))
			advance(i);
		char *tmp = ft_substr(data->input, start, *i - start);
		*i = start;
		add_token(data, i, tmp, VAR_EXPANSION);	
		free(tmp);
		return (true);
	}
	// arythmetic expansion
	else if (peek(data->input + *i, "$(", false))
	{
		int start = (*i)++;
		while (data->input[*i] != ')')
			advance(i);
		if (data->input[*i] == '\0')
			return (scanner_error(data, "error: unclosed expansion"));
		char *tmp = ft_substr(data->input, start, *i - start + 1);
		*i = start;
		add_token(data, i, tmp, COM_EXPANSION);
		free(tmp);
		return (true);
	}
	else
		return (false);
}

/****************************************/
/* redirections							*/
/****************************************/
bool	is_a_redirection(t_mini_data *data, int *i)
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
	else if (peek(data->input + *i, "<<", false))
	{
		add_token(data, i, "<<", DLESS);
		while (data->input[*i] && is_space(data->input[*i]))
			advance(i);
		if (data->input[*i] == '\0' || is_delimiter(data->input[*i]))
			return (scanner_error(data, "error: missing here-delim"));
		
		int start = *i;
		while ((data->input + *i) && !is_delimiter(data->input[*i]))
			advance(i);
		char *tmp = ft_substr(data->input, start, *i - start);
		if (!tmp)
			return (scanner_error(data, "error: malloc tmp failed"));
		*i = start;
		add_token(data, i, tmp, DLESS_DELIM);
		free(tmp);
		return (true);
	}
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
bool	add_tokenblock(t_mini_data *data, int *i, char delim, enum e_tokentype token_type)
{
	int start;
	char *tmp;

	start = (*i)++;
	while (data->input[*i] && data->input[*i] != delim)
		advance(i);
	if (data->input[*i] == '\0')
		return (scanner_error(data, "error: unclosed expression"));
	tmp = ft_substr(data->input, start, *i - start + 1);

	add_token(data, &start, tmp, token_type);
	*i = start;
	free(tmp);
	return (true);
}


bool is_a_block(t_mini_data *data, int *i)
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

// create a lexeme for flag in this conf -[a-zA-Z]
// else if (peek((data->input + i), "-", false) && is_alpha(data->input[i + 1]))
bool is_a_flag(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "-", false) && is_alpha(data->input[*i + 1]))
	{
		int start;
		char *tmp;
		
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

bool is_simple_operator(t_mini_data *data, int *i)
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

// pathname - NUMBER or identifier?
bool	is_a_string_thing(t_mini_data *data, int *i)
{
	if (isprint(data->input[*i]) && !filename_delimiter(data->input[*i]))
	{
		int start;
		char *tmp;

		start = *i;
		while (ft_isprint(data->input[*i]) && !filename_delimiter(data->input[*i]))
			advance(i);	
		tmp = ft_substr(data->input, start, *i - start);
		if (data->input[*i] == '<' || data->input[*i] == '>')
		{
			if (is_io_number(tmp))
				add_token(data, &start, tmp, IO_NUMBER);
		}
		else 
		{
			debug("identifier: -%s-", tmp);
			//check for reserved words and builtin first which they will be added 
			// automatically if the check is true
			if (!is_reserved(data, tmp, &start) && \
			!is_builtin(data, tmp, &start) &&\
			!is_true_false(data, tmp, &start))
			{
				// check if it is a path name
				if (ft_strchr(tmp, '/') || peek(tmp, ".", false) || peek(tmp, "./", false) || peek(tmp, "../", false) || peek(tmp, "~/", false) || peek(tmp, "~+", false))
					add_token(data, &start, tmp, PATHNAME);
				// if not a path name maybe a number?
				else if (str_is_number(tmp))
					add_token(data, &start, tmp, NUMBER);
				//if not a number maybe a variable name or anything else!
				else
					add_token(data, &start, tmp, WORD);
			}
		}
		free(tmp);
		// *i = start;
		return (true);
	}
	return (false);
}
/*

*/
bool	got_tokens(t_mini_data *data, int *i)
{
	if (is_a_control_operator(data, i) || is_a_math_op(data, i) || \
	is_a_hist_expansion(data, i) || is_a_dollar_exp(data, i) || \
	is_some_semicolons(data, i) || is_a_redirection(data, i) || \
	is_a_redirection(data, i) || is_simple_operator(data, i) ||\
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
t_list *tokenizer(t_mini_data *data)
{
	int i;
	
	i = 0;
	data->token_list = NULL;
	debug("scanning input: %s of num char %d", data->input, (int)ft_strlen(data->input));
	data->exit_status = 0;
	while (i < (int)ft_strlen(data->input) && data->exit_status == 0)
	{
		// hash # case the rest of the string will be a comment but we dont create a token, we ignore
		// else if (peek((data->input + i), "#", false))
		if (peek(data->input + i, "#", false))
			break;
		// extract tokens
		else if (got_tokens(data, &i))
		{
			debug("extracted token\n");
			continue;
		}

		else if (is_space(data->input[i]))
			i++;
		// this is just in case! but especially for debug I want to know if I get 
		// a character that I do not recognize so I can add it. maybe in production I should better ignore 
		// at least for the eval!
		else
		{
			i++;
			data->exit_status = 1;
			data->scanner_err_str = "error: token not recognized character";
			debug("error: token not recognized character\n");
		}
	}
	if (data->exit_status != 0)
	{
		debug("error: exit status %d\n", data->exit_status);
		ft_lstclear(&data->token_list, free_token);
		return (NULL);
	}
	// do a last parentheses check!
	return data->token_list;
}
