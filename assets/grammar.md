# Minishell grammar:

After a few days of research, I have come up with a grammar for the minishell project.  

```
list      -> pipeline (";" | "&" | "&&" | "||") pipeline)* [";"] | ["&"] ["\n"]
			| "(" list ")";
pipeline ->  command  (("|" | "|&" | ";" | "&&" | "||" )command)* ;
			| "(" list ")";
command ->  	  simple_command 
				| builtin 
				| DLESS 
				| redirection
				| [time [-p]] [!] expression
				| "(" list ")";

simple_command 	-> name (args)* ;
builtin 		-> name (args)* ; 
redirection 	-> expression ( "<" | ">" | ">>" | "2>" | "&> | &>> | 2>> | <> | >|") expression; 
DLESS 			-> expression "<<" delimiter newline content delimiter;

delimiter -> STRING;
content -> MULTIPLE_LINE_TEXT;
flags 	-> FLAGS;
name 	-> WORD | COM_EXPANSION | VAR_EXPANSION;
args 	-> FLAGS | WORD | STRING | QUOTED_STRING | SIMPLE_QUOTED_STRING | VAR_EXPANSION | EXPR_EXPANSION;
```

The operators "&&" and "||" shall have equal precedence and shall be evaluated with left associativity. For example, both of the following commands write solely bar to standard output:

false && echo foo || echo bar // false && echo foo is false so echo bar is executed
true || echo foo && echo bar // true || echo foo is true so echo bar is executed


For this subject:
`<`: Redirects standard input from a file.
`>`: Redirects standard output to a file, overwriting the file if it exists.
`>>`: Redirects standard output to a file, appending to the file if it exists.
`<<`: Here Document

There are a few more redirection operators that you might encounter but we will not need to implement them:
`2>`: Redirects standard error to a file, overwriting the file if it exists.
`&>`: Redirects both standard output and standard error to a file, overwriting
 the file if it exists.
`2>>`: Redirects standard error to a file, appending to the file if it exists.
`<>`: Opens a file for both reading and writing.
`>|`: Redirects standard output to a file, overwriting the file even if 
the noclobber option has been set in the shell.
`&>>`: Redirects both standard output and standard error to a file, 
appending to the file if it exists.

we will implement a parser method called "recursive descent,"
which is a top-down parser. 