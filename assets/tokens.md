# Tokens

looking at the bash and shell manuals, I have come up with a list of tokens that are meaningful to us. 
I tried to compile an exhaustive list of tokens that are used in bash and shell scripts.
As the project progresses I cometo realize that some of these tokens are not needed and probably some are even missing. 
I ill start with the actual list with a description. This is also consider the shell as a scripting language and not just an interactive shell.

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
EXPR_EXPANSION, // $(command) or $((arythm expression))
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
COMMA, DOT, MINUS, PLUS, SLASH, STAR, BANG_EQUAL, BANG,
// for the history expansion
BANG_BANG,BANG_DIGIT, BANG_HYPHEN_DIGIT, BANG_ALPHA, BANG_QUESTION_ALPHA,
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
HASH, 

COMMENT, starting with # and ending with a newline are ignored!
CARET, PERCENT, 
TILDE, used in the context of home directory expansion
EOF_TOK,  
COMMAND,
NULL_TOKEN
// to add ‘$’, ‘`’, ‘"’, ‘\’,The special parameters ‘*’ and ‘@’ have special meaning when in double quotes


## What do we need in the end?

