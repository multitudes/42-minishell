/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:47:11 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/05 14:22:43 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 ``\t''``\n''``\v''``\f''``\r''`` -> is space 
*/
bool is_space(const char c)
{
	if (c == 0)
		return (false);
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

/*
string compare case insensitive
*/
int strncicmp(char const *a, char const *b, int n)
{
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
*/
bool is_delimiter(const char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || 
        ch == '/' || ch == ',' || ch == ';' || ch == '>' || 
        ch == '<' || ch == '=' || ch == '(' || ch == ')' || 
        ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
        ch == '|' || ch == '&' || ch == '$' || ch == '`' ||
        ch == '"' || ch == '\'' || ch == '\\' || ch == '!' ||
        ch == '~' || ch == '^' || ch == '%' || ch == '\t' ||
        ch == '\n' || ch == '\v' || ch == '\f' || ch == '\r' || ch == EOF || ch == '\0')
		return (true);
    return (false);
}

/*
ok these char cannot be in a file name.. even if officially
it is recommended to use only the 65 characters [-._a-zA-Z0-9]
for files and directories ... touch w$orld is creating w because dereferencing 
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
The | character is a pipe in bash, which is used to pass the output 
of one command as input to another command.
In this case, touch w-+*}\,[]=w{ is being 
run as one command, and orld is being interpreted as a 
separate command.
The output of touch w-+*}\,[]=w{ (which is nothing, 
because touch doesn't produce any output) is being passed as input to orld.
However, orld is not a valid command, 
so bash gives an error message: bash: orld: command not found.
The [1]+ Done touch w-+*}\,[]=w{ message is from bash's job 
control system. It's telling you that the touch w-+*}\,[]=w{ command has finished running.
If you want to create a file with a | in the name, you'll need to 
escape it with a backslash (\|), or enclose the filename in quotes.  
However, it's generally best to avoid such characters in filenames, 
as they can cause confusion and errors.

touch w-+*},[]=w{\o!rld bash: !rld: event not found

The ! character has a special meaning in many Unix and Linux shells, 
including bash. It is used for history expansion, 
allowing you to re-run previous commands.
In your command touch w-+*}\,[]=w{\o!rld, the ! character 
is causing bash to attempt to perform history expansion
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
Returns 'true' if the character is char a letter or a digit or underscore.
btw This 65-character set, [-._a-zA-Z0-9], 
is referred to in SUSv3 as the portable filename character set.
*/
bool is_alpha(char c) 
{
	return ((c >= 'a' && c <= 'z') || \
    (c >= 'A' && c <= 'Z') || c == '_');
}

/*
part of the check for identifiers
*/
bool is_alnum(char c) 
{
	return is_alpha(c) || is_digit(c);
}

/*
either is alnum including the _ or is a . / ~
*/
bool is_pathname(char c) 
{
	return is_alnum(c) || c == '.' || c == '-' || c == '/' || c == '~' ;
}

/*
peek wil look ahead to see if my string is beginning with a word which 
is an identifier
*/
bool	peek(const char *input, const char *identifier, bool end_space)
{
	int i;
	int n;
	
	i = 0;
	n = ft_strlen(identifier);
	while (i < n && input[i] == identifier[i])
		i++;
	if (end_space)
	{
		if (i != n && input[i] != ' ' && input[i] != '\0')
			return (false);
	}
	// debug("peeking %s i is %d n is %d", identifier, i, n);
	if (i != n)
		return (false);
	return (true);
}

/*
 mh.. just checked again in the bash and maybe the function below is not necessary? TODO
 bash lets me create a var while and print it with $while... 	
 so not for identifier.  they cannot be used for commands...!
 IF, THEN, ELSE, ELIF, FI, DO, DONE, WHILE, UNTIL, FOR, CASE, ESAC, SELECT, FUNCTION,
*/
bool is_reserved(t_list **token_list, char *identifier,int *start)
{
	if (strncicmp(identifier, "if", 3) == 0)
		ft_lstadd_back(token_list, create_token(IF, "if", start));
	else if (strncicmp(identifier, "then", 5) == 0)
		ft_lstadd_back(token_list, create_token(THEN, "then", start));
	else if (strncicmp(identifier, "else", 5) == 0)
		ft_lstadd_back(token_list, create_token(ELSE, "else", start));
	else if (strncicmp(identifier, "elif", 5) == 0)
		ft_lstadd_back(token_list, create_token(ELIF, "elif", start));
	else if (strncicmp(identifier, "fi", 3) == 0)
		ft_lstadd_back(token_list, create_token(FI, "fi", start));
	else if (strncicmp(identifier, "do", 3) == 0)
		ft_lstadd_back(token_list, create_token(DO, "do", start));
	else if (strncicmp(identifier, "done", 5) == 0)
		ft_lstadd_back(token_list, create_token(DONE, "done", start));
	else if (strncicmp(identifier, "while", 6) == 0)
		ft_lstadd_back(token_list, create_token(WHILE, "while", start));
	else if (strncicmp(identifier, "until", 6) == 0)
		ft_lstadd_back(token_list, create_token(UNTIL, "until", start));
	else if (strncicmp(identifier, "for", 4) == 0)
		ft_lstadd_back(token_list, create_token(FOR, "for", start));
	else if (strncicmp(identifier, "case", 5) == 0)
		ft_lstadd_back(token_list, create_token(CASE, "case", start));
	else if (strncicmp(identifier, "esac", 5) == 0)
		ft_lstadd_back(token_list, create_token(ESAC, "esac", start));
	else if (strncicmp(identifier, "select", 7) == 0)
		ft_lstadd_back(token_list, create_token(SELECT, "select", start));
	else if (strncicmp(identifier, "function", 8) == 0)
		ft_lstadd_back(token_list, create_token(FUNCTION, "function", start));
	// else if (strncimp(identifier, "2>>", 3))
	// 	ft_lstadd_back(token_list, create_token(REDIRECT_ERR_APP, "2>>", start));
	// else if (strncimp(identifier, "2>", 2))
	// 	ft_lstadd_back(token_list, create_token(REDIRECT_ERR, "2>", start));
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
	strncicmp(id, ":", 2) == 0)
		return (true);
	return (false);
}
/*
is_builtin checks if the identifier is a builtin command
*/
bool is_builtin(t_list **token_list, char *identifier,int *start)
{
	if (strncicmp(identifier, "echo", 5) == 0)
		ft_lstadd_back(token_list, create_token(BUILTIN, "echo", start));
	else if (strncicmp(identifier, "cd", 3) == 0)
		ft_lstadd_back(token_list, create_token(BUILTIN, "cd", start));
	else if (strncicmp(identifier, "export", 7) == 0)
		ft_lstadd_back(token_list, create_token(BUILTIN, "export", start));
	else if (strncicmp(identifier, "unset", 6) == 0)
		ft_lstadd_back(token_list, create_token(BUILTIN, "unset", start));
	else if (strncicmp(identifier, "env", 4) == 0)
		ft_lstadd_back(token_list, create_token(BUILTIN, "env", start));
	else if (strncicmp(identifier, "exit", 3) == 0)
		ft_lstadd_back(token_list, create_token(BUILTIN, "exit", start));
	else if (strncicmp(identifier, "pwd", 3) == 0)
		ft_lstadd_back(token_list, create_token(BUILTIN, "pwd", start));
	else if (strncicmp(identifier, ".", 2) == 0)
		ft_lstadd_back(token_list, create_token(BUILTIN, ".", start));
	else if (not_implemented_builtin(identifier))
		ft_lstadd_back(token_list, create_token(BUILTIN, identifier, start));
	else
		return false;
	return true;
}
/*
utility 
*/
void	print_token_description(const t_list *current) 
{
	t_token *token;

	token = (t_token *)(current)->content;
	debug("token type :%u, lexeme: %s position %d", token->type, token->lexeme, token->start);
	// if (token->type != SPACE && token->type != NEWLINE_TOK)
	// 	debug("%s", token->lexeme);
	return ;
}

void print_token_list(t_list *token_list)
{
	t_list *current;

	current = token_list;
	while (current != NULL)
	{
		print_token_description(current);
		current = current->next;
	}
	return ;
}
/*
Too many vars for norminette so I made a struct
This is only to check if the parentheses and braces etc are
closing. If not I should not continue with the execution
*/
void init_open_close(t_open_close *open_close)
{
	open_close->paren = 0;
	open_close->curlys = 0;
	open_close->quotes = 0;
	open_close->single_quotes = 0;
	open_close->backslash = 0;
	open_close->square_brackets = 0;
}

/* 
will go through the list of tokens and check if the paren are closed and the quotes are closed
*/
int	check_paren_quotes(t_list *token_list)
{
	t_open_close open_close;
	t_list *current;
	t_token *token;

	init_open_close(&open_close);
	current = token_list;
	while (current)
	{
		token = (t_token *)current->content;
		// debug("checking token %s\n", token->lexeme);
		if (token->type == LEFT_CURLY)
			open_close.curlys++;
		else if (token->type == RIGHT_CURLY)
			open_close.curlys--;
		else if (token->type == QUOTE)
			open_close.quotes++;
		else if (token->type == SINGLE_QUOTE)
			open_close.single_quotes++;
		else if (token->type == LEFT_SQUARE)
			open_close.square_brackets++;
		else if (token->type == RIGHT_SQUARE)
			open_close.square_brackets--;
		else if (token->type == LEFT_PAREN)
			open_close.paren++;
		else if (token->type == RIGHT_PAREN)
			open_close.paren--;
		// else if (token->type == BACKSLASH)
		// 	open_close.backslash++;
		if (open_close.curlys < 0 || open_close.square_brackets < 0 || open_close.paren < 0)
			break;
		current = current->next;
	}
	debug("open close backslash = %d\n", open_close.backslash);
	if (open_close.paren != 0)
		return (err_stop("error: unclosed parenthesis\n"));
	else if (open_close.curlys != 0)
		return (err_stop("error: unclosed curly brackets\n"));
	else if (open_close.quotes % 2 != 0)
		return (err_stop("error: unclosed quotes\n"));
	else if (open_close.single_quotes % 2 != 0)
		return (err_stop("error: unclosed single quotes\n"));
	else if (open_close.square_brackets != 0)
		return (err_stop("error: unclosed square brackets\n"));
	// else if (open_close.backslash % 2 != 0)
	// 	return (err_stop("error: unclosed backslash\n"));
	return (1);
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
including numbers preceded by a - or + 
and with and without a dot
*/
int str_is_number(const char *identifier)
{
    int dot_seen = 0;

    if ((*identifier == '-' || *identifier == '+') && is_digit(*(identifier + 1)))
        identifier++;

    while (*identifier)
    {
        if (*identifier == '.')
        {
            if (dot_seen) // if we've already seen a dot, it's not a valid number
                return 0;
            dot_seen = 1;
        }
        else if (!is_digit(*identifier))
            return 0;
        identifier++;
    }

    return 1;
}


int str_is_alphanum(const char *str)
{
	while (*str)
		if (!is_alnum(*(str++)))
			return (0);
	return (1);
}

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
scanning function
Returns a token list which is a linked list of t_list type
note: we dont need to action some of them but fir completeness I added 
as much as I could. for instance we do not action the last & on a command
or redirections like 
*/
t_list *tokenizer(t_mini_data *data)
{
	int i;
	char *tmp;
	t_list *token;
	t_list *token_list;
	const char *input;
	
	i = 0;
	input = data->input;
	debug("scanning input: %s", input);
	token_list = NULL;
	while (i < (int)ft_strlen(input))
	{
	 	if (peek(input, "||", false))
		{
			token = create_token(OR_TOK, "||", &i);
			if (token)
				ft_lstadd_back(&token_list, token);
			else
			{
				//free
				return (NULL);
			} 
		}
		else if (input[i] == '|' && input[i + 1] == '&')
			ft_lstadd_back(&token_list, create_token(PIPE_AND, "|&", &i));
		else if (input[i] == '&' && input[i + 1] == '&')
			ft_lstadd_back(&token_list, create_token(AND_AND, "&&", &i));
		else if (input[i] == ';')
			ft_lstadd_back(&token_list, create_token(SEMI, ";", &i));
		else if (input[i] == ';' && input[i + 1] == '&')
			ft_lstadd_back(&token_list, create_token(SEMI_AND, ";&", &i));
		else if (input[i] == ';' && input[i + 1] == ';')
			ft_lstadd_back(&token_list, create_token(SEMI_SEMI, ";;", &i));
		else if (input[i] == ';' && input[i + 1] == ';' && input[i + 2] == '&')
			ft_lstadd_back(&token_list, create_token(SEMI_SEMI_AND, ";;&", &i));
		// wanna create an expression token
		else if (input[i] == '(')
		{
			int start = i;
			i++;
			while (input[i] != ')' && input[i] != '\0')
				i++;
			if (input[i] == '\0')
			{
				debug("error: unclosed parentheses\n");
				return (NULL);
			}
			tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(EXPRESSION, tmp, &start));
			free(tmp);
			i++;
		}
		else if (input[i] == '-' && input[i + 1] == '-' && input[i + 2] == ' ')
			ft_lstadd_back(&token_list, create_token(MINUS_MINUS, "--", &i));
		else if (input[i] == '+' && input[i + 1] == '+' && input[i + 2] == ' ')
			ft_lstadd_back(&token_list, create_token(PLUS_PLUS, "++", &i));
		else if (input[i] == '-' && input[i + 1] == '=')
			ft_lstadd_back(&token_list, create_token(MINUS_EQUAL, "-=", &i));
		else if (input[i] == '+' && input[i + 1] == '=')
			ft_lstadd_back(&token_list, create_token(PLUS_EQUAL, "+=", &i));
		else if (input[i] == '/' && input[i + 1] == '=')
			ft_lstadd_back(&token_list, create_token(SLASH_EQUAL, "/=", &i));
		else if (input[i] == '*' && input[i + 1] == '=')
			ft_lstadd_back(&token_list, create_token(STAR_EQUAL, "*=", &i));
		
		/*
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
		else if (input[i] == '!' && input[i + 1] == '!') 
			ft_lstadd_back(&token_list, create_token(BANG_BANG, "!!", &i));
		else if (input[i] == '!' && is_digit(input[i + 1]))
		{
			int start = i;
			while (is_digit(input[i + 1]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(BANG_DIGIT, tmp, &start));
			free(tmp);
			i++;
		}
		else if (input[i] == '!' && input[i + 1] == '-' && is_digit(input[i + 2]))
		{
			int start = i++;
			while (is_digit(input[i + 1]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(BANG_HYPHEN_DIGIT, tmp, &start));
			free(tmp);
			i++;
		}
		else if (input[i] == '!' && is_alnum(input[i + 1]))
		{
			int start = i;
			while (is_alpha(input[i + 1]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(BANG_ALPHA, tmp, &start));
			free(tmp);
			i++;
		}
		else if (input[i] == '!' && input[i + 1] == '?' && is_alnum(input[i + 2]))
		{
			int start = i++;
			i++;	
			while (is_alnum(input[i]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(BANG_QUESTION_ALPHA, tmp, &start));
			free(tmp);
			i++;
		}
		else if (input[i] == '!')
			ft_lstadd_back(&token_list, create_token(BANG, "!", &i));
		else if (input[i] == '=')
			ft_lstadd_back(&token_list, create_token(EQUAL, "=", &i));
		
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

		else if (input[i] == '$' && input[i + 1] == '?')
			ft_lstadd_back(&token_list, create_token(DOLLAR_QUESTION, "$?", &i));
		else if (input[i] == '$' && input[i + 1] == '$')
			ft_lstadd_back(&token_list, create_token(DOLLAR_DOLLAR, "$$", &i));
		else if (input[i] == '$' && input[i + 1] == '*')
			ft_lstadd_back(&token_list, create_token(DOLLAR_STAR, "$*", &i));
		else if (input[i] == '$' && input[i + 1] == '@')
			ft_lstadd_back(&token_list, create_token(DOLLAR_AT, "$@", &i));
		else if (input[i] == '$' && input[i + 1] == '#')
			ft_lstadd_back(&token_list, create_token(DOLLAR_HASH, "$#", &i));
		else if (input[i] == '$' && input[i + 1] == '!')
			ft_lstadd_back(&token_list, create_token(DOLLAR_BANG, "$!", &i));
		else if (input[i] == '$' && input[i + 1] == '-')
			ft_lstadd_back(&token_list, create_token(DOLLAR_HYPHEN, "$-", &i));
		else if (input[i] == '$' && (input[i + 1] == '0' || input[i + 1] == '1' || \
		input[i + 1] == '2' || input[i + 1] == '3' || input[i + 1] == '4' || \
		input[i + 1] == '5' || input[i + 1] == '6' || input[i + 1] == '7' || \
		input[i + 1] == '8' || input[i + 1] == '9'))
		{
			int start = i;
			while (is_digit(input[i + 1]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(DOLLAR_DIGIT, tmp, &start));
			free(tmp);
			i++;
		}
		// expansion ${parameter} or $parameter or $(command) or $((arythm expression))
		else if (input[i] == '$' && (input[i + 1] == '{'))
		{
			int start = i++;
			while (input[i] != '}' && input[i] != '\0')
				i++;
			if (input[i] == '\0')
			{
				debug("error: unclosed expansion\n");
				return NULL;
			}
			tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(VAR_EXPANSION, tmp, &start));
			free(tmp);
			i++;
		}
		// Parameter names in bash can only contain alphanumeric 
		// characters or underscores, and must start with a letter or underscore.
		else if (input[i] == '$' && is_alnum(input[i + 1]))
		{
			int start = i++;
			while (is_alnum(input[i]))
				i++;
			tmp = ft_substr(input, start, i - start);
			ft_lstadd_back(&token_list, create_token(VAR_EXPANSION, tmp, &start));
			free(tmp);
			i++;
		}
		else if (input[i] == '$' && input[i + 1] == '(' && input[i + 2] != '(')
		{
			int start = i++;
			while (input[i] != ')' && input[i] != '\0')
				i++;
			if (input[i] == '\0')
			{
				debug("error: unclosed expansion\n");
				return NULL;
			}
			tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(COM_EXPANSION, tmp, &start));
			free(tmp);
			i++;
		}
		// Command substitution
   		// COMMAND_SUBSTITUTION, // '$(command)' or '`command`'
		else if (input[i] == '`')
		{
			int start = i++;
			while (input[i] != '`' && input[i] != '\0')
				i++;
			if (input[i] == '\0')
			{
				debug("error: unclosed expansion\n");
				return NULL;
			}
			tmp = ft_substr(input, start, i - start + 1);
			ft_lstadd_back(&token_list, create_token(COM_EXPANSION, tmp, &start));
			free(tmp);
			i++;
		}
		else if (input[i] == '$' && input[i + 1] == '(' && input[i + 2] == '(')
		{
			int start = i++;
			while (input[i] != ')' && input[i + 1] != ')' && input[i] != '\0')
				i++;
			if (input[i] == '\0')
			{
				debug("error: unclosed expansion\n");
				return NULL;
			}
			tmp = ft_substr(input, start, i - start + 3);
			ft_lstadd_back(&token_list, create_token(EXPR_EXPANSION, tmp, &start));
			free(tmp);
			i += 3;
		}
		// 
		// >|
		else if (input[i] == '>' && input[i + 1] == '|')
			ft_lstadd_back(&token_list, create_token(REDIRECT_OUT, ">|", &i));
		//2>>
		else if (input[i] == '2' && input[i + 1] == '>' && input[i + 2] == '>')
			ft_lstadd_back(&token_list, create_token(REDIRECT_ERR_APP, "2>>", &i));
		else if (input[i] == '2' && input[i + 1] == '>' && input[i + 2] == '&')
			ft_lstadd_back(&token_list, create_token(REDIRECT_ERR_FD, "2>&", &i));
		else if (input[i] == '2' && input[i + 1] == '>')
			ft_lstadd_back(&token_list, create_token(REDIRECT_ERR, "2>", &i));
		else if (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '&')
			ft_lstadd_back(&token_list, create_token(REDIRECT_BOTH_APP, ">>&", &i));
		else if (input[i] == '&' && input[i + 1] == '>' && input[i + 2] == '>')
			ft_lstadd_back(&token_list, create_token(REDIRECT_OUT_APP, "&>>", &i));
		else if (input[i] == '>' && input[i + 1] == '&' && input[i + 2] == '>')
			ft_lstadd_back(&token_list, create_token(GREATER_AND_GREATER , ">&>", &i));
		else if (input[i] == '&' && input[i + 1] == '>')
			ft_lstadd_back(&token_list, create_token(REDIRECT_BOTH, "&>", &i));
		else if (input[i] == '<' && input[i + 1] == '&')
			ft_lstadd_back(&token_list, create_token(LESS_AND, "<&", &i));
		else if (input[i] == '>' && input[i + 1] == '&')
			ft_lstadd_back(&token_list, create_token(REDIRECT_FD, ">&", &i));
		else if (input[i] == '<' && input[i + 1] == '>')
			ft_lstadd_back(&token_list, create_token(READ_WRITE_MODE, "<>", &i));
		else if (input[i] == '>' && input[i + 1] == '>')
			ft_lstadd_back(&token_list, create_token(APPEND, ">>", &i));
		// heredocs << dont need to be preceded or followed by a space 
		
		else if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
		{
			ft_lstadd_back(&token_list, create_token(HEREDOC, "<<", &i));
			debug("heredoc tokens");
			while (input[i] && is_space(input[i]))
				i++;
			if (input[i] == '\0' || is_delimiter(input[i]))
			{
				debug("error: unclosed heredoc\n");
				return (NULL);
			}
			int start = i;
			while (input[i] && !is_delimiter(input[i]))
				i++;
			tmp = ft_substr(input, start, i - start);
			ft_lstadd_back(&token_list, create_token(HEREDOC_DELIM, tmp, &start));
			free(tmp);
			// debug("ADD HEREDOC CONTENT CODE!");
		}
		else if (input[i] == '>' && input[i + 1] == '=')
			ft_lstadd_back(&token_list, create_token(GREATER_EQUAL, ">=", &i));
		else if (input[i] == '<' && input[i + 1] == '=')
			ft_lstadd_back(&token_list, create_token(LESS_EQUAL, "<=", &i));
		else if (input[i] == '!' && input[i + 1] == '=')
			ft_lstadd_back(&token_list, create_token(BANG_EQUAL, "!=", &i));
		else if (input[i] == '=' && input[i + 1] == '=')
			ft_lstadd_back(&token_list, create_token(EQUAL_EQUAL, "==", &i));
		else if (input[i] == '>')
			ft_lstadd_back(&token_list, create_token(REDIRECT_OUT, ">", &i));
		else if (input[i] == '<')
			ft_lstadd_back(&token_list, create_token(REDIRECT_IN, "<", &i));
		else if (input[i] == ',')
			ft_lstadd_back(&token_list, create_token(COMMA, ",", &i));
		else if (input[i] == '-' && input[i + 1] == ' ')
			ft_lstadd_back(&token_list, create_token(MINUS, "-", &i));
		else if (input[i] == '+')
			ft_lstadd_back(&token_list, create_token(PLUS, "+", &i));
		/* 
		is always a directory in bash so might as well get a lexeme like this
		starts with ~ or / but a directory cannot start with - (or ~)
		*/
		// else if (input[i] == '/' || input[i] == '~' )
		// {
		// 	int start = i;

		// 	while (is_pathname(input[i]) && !((input[i] == '/') && input[i + 1] == '-')  && !((input[i] == '/') && input[i + 1] == '~'))
		// 		i++;
		// 	// thid is a first check but it is more complicated than that - TODO
		// 	if (is_pathname(input[i]))
		// 	{
		// 		debug("error: invalid path\n");
		// 		return (NULL);
		// 	}
		// 	tmp = ft_substr(input, start, i - start);
		// 	// no expansion in those unless they are part of a double quoted string
		// 	// expand later $() ${} and $VAR and '...'  and ` ... `  ?
			
		// 	ft_lstadd_back(&token_list, create_token(PATHNAME, tmp, &start));
		// 	free(tmp);
		// 	i++;	
		// }
		else if (input[i] == '*')
			ft_lstadd_back(&token_list, create_token(STAR, "*", &i));
		else if (input[i] == '?')
			ft_lstadd_back(&token_list, create_token(QUESTION, "?", &i));
		// if there is a double quote I need to create a string token
		else if (input[i] == '|' && (input[i + 1] != '|' || input[i+1] != '&'))
			ft_lstadd_back(&token_list, create_token(PIPE, "|", &i));
		//SINGLE_QUOTED_STRING, single quoted string 'string'
		else if (input[i] == '\'')
		{
			int start = i++;

			while (input[i] != '\'' && input[i] != '\0')
				i++;
			if (input[i] == '\0')
			{
				debug("error: unclosed single quotes\n");
				return (NULL);
			}
			tmp = ft_substr(input, start, i - start + 1);
			// no expansion in those unless they are part of a double quoted string
			// expand later $() ${} and $VAR and '...'  and ` ... `  ?
			
			ft_lstadd_back(&token_list, create_token(SINGLE_QUOTED_STRING, tmp, &start));
			free(tmp);
			i++;
		}
		else if (input[i] == '"')
		{
			int start = i++;

			while (input[i] != '"' && input[i] != '\0')
				i++;
			if (input[i] == '\0')
			{
				debug("error: unclosed quotes\n");
				return (NULL);
			}
			tmp = ft_substr(input, start, i - start + 1);
			// expand later $() ${} and $VAR and '...'  and ` ... `  ?
			
			ft_lstadd_back(&token_list, create_token(QUOTED_STRING, tmp, &start));
			free(tmp);
			i++;
		}
		// else if (input[i] == '\\')
		// 	ft_lstadd_back(&token_list, create_token(BACKSLASH, "\\", &i));
		else if (input[i] == EOF)
			ft_lstadd_back(&token_list, create_token(EOF_TOK, "EOF", &i));
		else if (input[i] == '-' && is_alpha(input[i + 1]))
		{
			// create a lexeme for flag in this conf -[a-zA-Z]
			int start = i;
			i++;
			if (is_alpha(input[i]))
			{
				while (is_alpha(input[i]))
					i++;
				tmp = ft_substr(input, start, i - start);
				ft_lstadd_back(&token_list, create_token(FLAGS, tmp, &start));
			}
			free(tmp);
			i++;
		}
		// else if (is_digit(input[i]))
		// {
		// 	int start = i;
		// 	while (is_digit(input[i]))
		// 	{
		// 		i++;
		// 		if (input[i] == '.')
		// 		{
		// 			i++;
		// 			while (is_digit(input[i]))
		// 				i++;
		// 		}
		// 	}
		// 	// add the rest of the string as a digit
		// 	tmp = ft_substr(input, start, i - start);
		// 	ft_lstadd_back(&token_list, create_token(NUMBER, tmp, &start));
		// 	if (!is_delimiter(input[i]))
		// 		debug("error: invalid number missing delimiter\n");
		// 	free(tmp);
		// }
		else if (input[i] == '&')
			ft_lstadd_back(&token_list, create_token(AND_TOK, "&", &i));
		else if (input[i] == '^')
			ft_lstadd_back(&token_list, create_token(CARET, "^", &i));
		else if (input[i] == '%')
			ft_lstadd_back(&token_list, create_token(PERCENT, "%", &i));
		else if (input[i] == '~')
			ft_lstadd_back(&token_list, create_token(TILDE, "~", &i));
		else if (input[i] == '$')
			ft_lstadd_back(&token_list, create_token(DOLLAR, "$", &i));
		// hash # case the rest of the string will be a comment
		else if (input[i] == '#')
		{
			// add the rest of the string as a comment
			while (input[i] != '\0')
				i++;
			break ;
		}
		// try pathname again!
		else if (isprint(input[i]) && !filename_delimiter(input[i]))
		{
			// debug("- pathname - NUMBER or identifier? ");
			int start = i;
			while (ft_isprint(input[i]) && !filename_delimiter(input[i]) && !peek(&input[i], "2>>", false) && !peek(&input[i], "2>", false) && !peek(&input[i], "2>&", false))
				i++;
			if (i != start)
			{			
				char *identifier = ft_substr(input, start, i - start);
				debug("identifier: -%s-", identifier);
				//check for reserved words
				if (!is_reserved(&token_list,identifier,&start) && !is_builtin(&token_list, identifier, &start))
				{
					if (ft_strchr(identifier, '/') || peek(identifier, " .", false) || peek(identifier, "./", false) || peek(identifier, "../", false) || peek(identifier, "~/", false) || peek(identifier, "~+", false))
						ft_lstadd_back(&token_list, create_token(PATHNAME, identifier, &start));
					else if (str_is_number(identifier))
						ft_lstadd_back(&token_list, create_token(NUMBER, identifier, &start));
					// else if (is_comment(identifier))
					// 	ft_lstadd_back(&token_list, create_token(COMMENT, identifier, &start));
					// else if (str_is_alphanum(identifier))
					// 	ft_lstadd_back(&token_list, create_token(IDENTIFIER, identifier, &start));
					else
						ft_lstadd_back(&token_list, create_token(IDENTIFIER, identifier, &start));
				}
				free(identifier);
			}
		}
		// else if (is_alpha(input[i]))
		// {
		// 	// debug("is alpha\n");
		// 	int start = i;
		// 	while (is_alnum(input[i]))
		// 		i++;
		// 	// add the rest of the string as an identifier
		// 	char *identifier = ft_substr(input, start, i - start);
		// 	//check for reservee words
		// 	if (!is_reserved(&token_list,identifier,&start))
		// 		ft_lstadd_back(&token_list, create_token(IDENTIFIER, identifier, &start));
		// 	free(identifier);
		// }
		else if (input[i] == '.')
			ft_lstadd_back(&token_list, create_token(PATHNAME, ".", &i));
		else if (is_space(input[i]))
			i++;
		// else
		// 	i++;
	}	 
	// print_token_list(token_list);

	// analysing if the paren are closing and the quotes are closed
	// if not print an error message
	// if (!check_paren_quotes(token_list))
	// 	return (NULL);

	return token_list;
}
