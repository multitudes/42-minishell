t_list *scan_this(const char *input)
{
	debug("scanning input: %s", input);
	int i;
	char *tmp;
	t_list *token;
	t_list *token_list;

	i = 0;
	token = create_token(NULL_TOKEN, "", &i);
	token_list = NULL;

	while (i <= (int)ft_strlen(input))
	{
	 	if (input[i] == '|' && input[i + 1] == '|') 
			token = create_token(OR_TOK, "||", &i);
		else if (input[i] == '|' && input[i + 1] == '&')
			token = create_token(PIPE_AND, "|&", &i);
		else if (input[i] == '&' && input[i + 1] == '&')
			token = create_token(AND_AND, "&&", &i);
		else if (input[i] == ';')
			token = create_token(SEMI, ";", &i);
		/*
		for case statements - not implemented!
		*/
		else if (input[i] == ';' && input[i + 1] == '&')
			token = create_token(SEMI_AND, ";&", &i);
		else if (input[i] == ';' && input[i + 1] == ';')
			token = create_token(SEMI_SEMI, ";;", &i);
		else if (input[i] == ';' && input[i + 1] == ';' && input[i + 2] == '&')
			token = create_token(SEMI_SEMI_AND, ";;&", &i);
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
			token = create_token(EXPRESSION, tmp, &start);
			free(tmp);
			i++;
		}
		else if (input[i] == '-' && input[i + 1] == '-' && input[i + 2] == ' ')
			token = create_token(MINUS_MINUS, "--", &i);
		else if (input[i] == '+' && input[i + 1] == '+' && input[i + 2] == ' ')
			token = create_token(PLUS_PLUS, "++", &i);
		else if (input[i] == '-' && input[i + 1] == '=')
			token = create_token(MINUS_EQUAL, "-=", &i);
		else if (input[i] == '+' && input[i + 1] == '=')
			token = create_token(PLUS_EQUAL, "+=", &i);
		else if (input[i] == '/' && input[i + 1] == '=')
			token = create_token(SLASH_EQUAL, "/=", &i);
		else if (input[i] == '*' && input[i + 1] == '=')
			token = create_token(STAR_EQUAL, "*=", &i);
		
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
			token = create_token(BANG_BANG, "!!", &i);
		else if (input[i] == '!' && is_digit(input[i + 1]))
		{
			int start = i;
			while (is_digit(input[i + 1]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			token = create_token(BANG_DIGIT, tmp, &start);
			free(tmp);
			i++;
		}
		else if (input[i] == '!' && input[i + 1] == '-' && is_digit(input[i + 2]))
		{
			int start = i++;
			while (is_digit(input[i + 1]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			token = create_token(BANG_HYPHEN_DIGIT, tmp, &start);
			free(tmp);
			i++;
		}
		else if (input[i] == '!' && is_alnum(input[i + 1]))
		{
			int start = i;
			while (is_alpha(input[i + 1]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			token = create_token(BANG_ALPHA, tmp, &start);
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
			token = create_token(BANG_QUESTION_ALPHA, tmp, &start);
			free(tmp);
			i++;
		}
		else if (input[i] == '!')
			token = create_token(BANG, "!", &i);
		else if (input[i] == '=')
			token = create_token(EQUAL, "=", &i);
		
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
			token = create_token(DOLLAR_QUESTION, "$?", &i);
		else if (input[i] == '$' && input[i + 1] == '$')
			token = create_token(DOLLAR_DOLLAR, "$$", &i);
		else if (input[i] == '$' && input[i + 1] == '*')
			token = create_token(DOLLAR_STAR, "$*", &i);
		else if (input[i] == '$' && input[i + 1] == '@')
			token = create_token(DOLLAR_AT, "$@", &i);
		else if (input[i] == '$' && input[i + 1] == '#')
			token = create_token(DOLLAR_HASH, "$#", &i);
		else if (input[i] == '$' && input[i + 1] == '!')
			token = create_token(DOLLAR_BANG, "$!", &i);
		else if (input[i] == '$' && input[i + 1] == '-')
			token = create_token(DOLLAR_HYPHEN, "$-", &i);
		else if (input[i] == '$' && (input[i + 1] == '0' || input[i + 1] == '1' || \
		input[i + 1] == '2' || input[i + 1] == '3' || input[i + 1] == '4' || \
		input[i + 1] == '5' || input[i + 1] == '6' || input[i + 1] == '7' || \
		input[i + 1] == '8' || input[i + 1] == '9'))
		{
			int start = i;
			while (is_digit(input[i + 1]))
				i++;
			char *tmp = ft_substr(input, start, i - start + 1);
			token = create_token(DOLLAR_DIGIT, tmp, &start);
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
			token = create_token(VAR_EXPANSION, tmp, &start);
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
			token = create_token(VAR_EXPANSION, tmp, &start);
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
			token = create_token(COM_EXPANSION, tmp, &start);
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
			token = create_token(COM_EXPANSION, tmp, &start);
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
			token = create_token(EXPR_EXPANSION, tmp, &start);
			free(tmp);
			i += 3;
		}
		// 
		// >|
		else if (input[i] == '>' && input[i + 1] == '|')
			token = create_token(REDIRECT_OUT, ">|", &i);
		//2>>
		else if (input[i] == '2' && input[i + 1] == '>' && input[i + 2] == '>')
			token = create_token(REDIRECT_ERR_APP, "2>>", &i);
		else if (input[i] == '2' && input[i + 1] == '>' && input[i + 2] == '&')
			token = create_token(REDIRECT_ERR_FD, "2>&", &i);
		else if (input[i] == '2' && input[i + 1] == '>')
			token = create_token(REDIRECT_ERR, "2>", &i);
		else if (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '&')
			token = create_token(REDIRECT_BOTH_APP, ">>&", &i);
		else if (input[i] == '&' && input[i + 1] == '>' && input[i + 2] == '>')
			token = create_token(REDIRECT_OUT_APP, "&>>", &i);
		else if (input[i] == '>' && input[i + 1] == '&' && input[i + 2] == '>')
			token = create_token(GREATER_AND_GREATER , ">&>", &i);
		else if (input[i] == '&' && input[i + 1] == '>')
			token = create_token(REDIRECT_BOTH, "&>", &i);
		else if (input[i] == '<' && input[i + 1] == '&')
			token = create_token(LESS_AND, "<&", &i);
		else if (input[i] == '>' && input[i + 1] == '&')
			token = create_token(REDIRECT_FD, ">&", &i);
		else if (input[i] == '<' && input[i + 1] == '>')
			token = create_token(READ_WRITE_MODE, "<>", &i);
		else if (input[i] == '>' && input[i + 1] == '>')
			token = create_token(APPEND, ">>", &i);
		// heredocs << dont need to be preceded or followed by a space 
		
		else if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
		{
			token = create_token(HEREDOC, "<<", &i);
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
			token = create_token(HEREDOC_DELIM, tmp, &start);
			free(tmp);
			// debug("ADD HEREDOC CONTENT CODE!");
		}
		else if (input[i] == '>' && input[i + 1] == '=')
			token = create_token(GREATER_EQUAL, ">=", &i);
		else if (input[i] == '<' && input[i + 1] == '=')
			token = create_token(LESS_EQUAL, "<=", &i);
		else if (input[i] == '!' && input[i + 1] == '=')
			token = create_token(BANG_EQUAL, "!=", &i);
		else if (input[i] == '=' && input[i + 1] == '=')
			token = create_token(EQUAL_EQUAL, "==", &i);
		else if (input[i] == '>')
			token = create_token(REDIRECT_OUT, ">", &i);
		else if (input[i] == '<')
			token = create_token(REDIRECT_IN, "<", &i);
		else if (input[i] == ',')
			token = create_token(COMMA, ",", &i);
		else if (input[i] == '-' && input[i + 1] == ' ')
			token = create_token(MINUS, "-", &i);
		else if (input[i] == '+')
			token = create_token(PLUS, "+", &i);
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
			
		// 	token = create_token(PATHNAME, tmp, &start));
		// 	free(tmp);
		// 	i++;	
		// }
		else if (input[i] == '*')
			token = create_token(STAR, "*", &i);
		else if (input[i] == '?')
			token = create_token(QUESTION, "?", &i);
		// if there is a double quote I need to create a string token
		else if (input[i] == '|' && (input[i + 1] != '|' || input[i+1] != '&'))
			token = create_token(PIPE, "|", &i);
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
			
			token = create_token(SINGLE_QUOTED_STRING, tmp, &start);
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
			
			token = create_token(QUOTED_STRING, tmp, &start);
			free(tmp);
			i++;
		}
		// else if (input[i] == '\\')
		// 	token = create_token(BACKSLASH, "\\", &i));
		else if (input[i] == EOF)
			token = create_token(EOF_TOK, "EOF", &i);
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
				token = create_token(FLAGS, tmp, &start);
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
		// 	token = create_token(NUMBER, tmp, &start));
		// 	if (!is_delimiter(input[i]))
		// 		debug("error: invalid number missing delimiter\n");
		// 	free(tmp);
		// }
		else if (input[i] == '&')
			token = create_token(AND_TOK, "&", &i);
		else if (input[i] == '^')
			token = create_token(CARET, "^", &i);
		else if (input[i] == '%')
			token = create_token(PERCENT, "%", &i);
		else if (input[i] == '~')
			token = create_token(TILDE, "~", &i);
		else if (input[i] == '$')
			token = create_token(DOLLAR, "$", &i);
		// hash # case the rest of the string will be a comment
		else if (input[i] == '#')
		{
			int start = i;
			// token = create_token(HASH, "#", &i));
			// add the rest of the string as a comment
			while (input[i] != '\0')
				i++;
			char *identifier = ft_substr(input, start, i - start);
			debug("identifier: -%s-\n", identifier);
			token = create_token(COMMENT, identifier, &start);
			free(identifier);
			break ;
		}
		// try pathname again!
		else if (isprint(input[i]) && !filename_delimiter(input[i]))
		{
			// debug("- pathname - NUMBER or identifier? ");
			int start = i;
			while (ft_isprint(input[i]) && !filename_delimiter(input[i]) && !peek(&input[i], "2>>") && !peek(&input[i], "2>") && !peek(&input[i], "2>&"))
				i++;
			if (i != start)
			{			
				char *identifier = ft_substr(input, start, i - start);
				debug("identifier: -%s-", identifier);
				//check for reserved words
				if (!is_reserved(&token_list,identifier,&start) && !is_builtin(&token_list, identifier, &start))
				{
					if (ft_strchr(identifier, '/'))
						token = create_token(PATHNAME, identifier, &start);
					else if (str_is_number(identifier))
						token = create_token(NUMBER, identifier, &start);
					// else if (is_comment(identifier))
					// 	token = create_token(COMMENT, identifier, &start));
					// else if (str_is_alphanum(identifier))
					// 	token = create_token(IDENTIFIER, identifier, &start));
					else
						token = create_token(IDENTIFIER, identifier, &start);
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
		// 		token = create_token(IDENTIFIER, identifier, &start));
		// 	free(identifier);
		// }
		else if (input[i] == '.')
			token = create_token(DOT, ".", &i);
		else if (is_space(input[i]))
			i++;
		else
			i++;
		if (token)
			{
				debug("token %s", (char*)token->content) ;
				ft_lstadd_back(&token_list, token);
			}
		else
		{
			debug("error: malloc invalid token\n");
			return (NULL);
		}
	}	 
	// print_token_list(token_list);

	// analysing if the paren are closing and the quotes are closed
	// if not print an error message
	// if (!check_paren_quotes(token_list))
	// 	return (NULL);

	return token_list;
}