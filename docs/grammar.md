# Minishell grammar:

After a few days of research, I have come up with a grammar for the minishell project.  

```bnf
list      -> pipeline (";" | "&" | "&&" | "||" pipeline)* [";"] | ["&"] ["\n"]
           | "(" list ")";
pipeline  -> command (( "|" | "|&" | ";" | "&&" | "||" ) command)* ;
           | "(" list ")";
command   -> simple_command 
           | builtin 
           | DLESS 
           | redirection
           | [time [-p]] [!] expression
           | "(" list ")";

simple_command -> name (args)* ;
builtin        -> name (args)* ; 
redirection    -> expression ( "<" | ">" | ">>" | "2>" | "&>" | "&>>" | "2>>" | "<>" | ">|") expression; 
DLESS          -> expression "<<" delimiter newline content delimiter;

delimiter -> STRING;
content   -> MULTIPLE_LINE_TEXT;
flags     -> FLAGS;
name      -> WORD | COM_EXPANSION | VAR_EXPANSION;
args      -> FLAGS | WORD | STRING | QUOTED_STRING | SIMPLE_QUOTED_STRING | VAR_EXPANSION | EXPR_EXPANSION;
```

The operators "&&" and "||" shall have equal precedence and shall be evaluated with left associativity. For example, both of the following commands write solely bar to standard output:

```bash
false && echo foo || echo bar  # false && echo foo is false so echo bar is executed
true || echo foo && echo bar   # true || echo foo is true so echo bar is executed
```

For this subject:
- `<`: Redirects standard input from a file.
- `>`: Redirects standard output to a file, overwriting the file if it exists.
- `>>`: Redirects standard output to a file, appending to the file if it exists.
- `<<`: Here Document

There are a few more redirection operators that you might encounter but we will not need to implement them:
- `2>`: Redirects standard error to a file, overwriting the file if it exists.
- `&>`: Redirects both standard output and standard error to a file, overwriting the file if it exists.
- `2>>`: Redirects standard error to a file, appending to the file if it exists.
- `<>`: Opens a file for both reading and writing.
- `>|`: Redirects standard output to a file, overwriting the file even if the noclobber option has been set in the shell.
- `&>>`: Redirects both standard output and standard error to a file, appending to the file if it exists.

We will implement a parser method called "recursive descent," which is a top-down parser. 

## Delimiters 

In bash, a delimiter is a character or a set of characters that separates different parts of the command line. The delimiters you've listed are a good start, but bash has a few more. Here's an expanded list:

- Space (`' '`)
- Tab (`'\t'`)
- Newline (`'\n'`)
- Semicolon (`;`)
- Pipe (`|`)
- Ampersand (`&`)
- Less than (`<`)
- Greater than (`>`)
- Open parenthesis (`(`)
- Close parenthesis (`)`)
- Open curly brace (`{`)
- Close curly brace (`}`)
- Open square bracket (`[`)
- Close square bracket (`]`)
- Dollar sign (`$`)
- Backtick (`` ` ``)
- Double quote (`"`)
- Single quote (`'`)
- Backslash (`\`)
- Equals (`=`)
- Plus (`+`)
- Minus (`-`)
- Asterisk (`*`)
- Slash (`/`)
- Comma (`,`)
- Exclamation mark (`!`)
- Tilde (`~`)
- Caret (`^`)
- Percent (`%`)

## {} and [] in bash scripts

They have specific meanings:

- `{}`: Curly braces are used in bash for variable expansion (`${variable}`), brace expansion (`{1..10}`), and to define blocks of code (like in if statements and functions).
- `[]`: Square brackets are used in bash for array indexing (`array[0]`), and to test conditions (`[ $a -lt 10 ]` or `[[ $a -lt 10 ]]`).

Here are some examples:
- Variable expansion: `echo ${variable}`
- Brace expansion: `echo {1..10}`
- Code block:
  ```bash
  if [ $a -lt 10 ]; then  
    echo "a is less than 10"
  fi
  ```
- Array indexing: `echo ${array[0]}`
- Test conditions: `if [ $a -lt 10 ]` or `if [[ $a -lt 10 ]]`

```bash
./myscript arg1 arg2 arg3
```

Then inside myscript, `$#` will be 3, because three arguments were passed to the script.

The `^` symbol in bash has a few different uses:

1. In regular expressions, `^` is used to denote the start of a line. For example, `^abc` matches any line that starts with "abc".
2. In parameter substitution, `${var^}` converts the first character of `$var` to uppercase.
3. In parameter substitution, `${var^^}` converts all characters of `$var` to uppercase.
4. In the `tr` command, `^` is used to denote a range of characters. For example, `tr A-Z a-z` converts uppercase letters to lowercase.
5. In the `tr` command, `^` is used to complement a set of characters when it's the first character in a set. For example, `tr -d '^0-9'` deletes all characters that are not digits.
6. In the `diff` command, `^` is used to denote lines that are different between two files.

The `<>` operator in bash is used for opening a file in read-write mode. Here's an example:

```bash
command <> file
```

This command will run `command`, with `file` opened in read-write mode on standard input.

```bash
command 1>>file 2>&1
```

This command will run `command`, and append both the stdout and stderr to `file`.
Bash 4 and later shortened to:

```bash
command &>>file
```

No, the symbols `;;`, `;&`, and `;;&` cannot be at the beginning of a command line in bash.
These symbols are used in the context of a case statement in bash scripting:

- `;;` is used to end each case in a case statement.
- `;&` allows execution to continue with the next case clause, rather than exiting the case statement.
- `;;&` allows the shell to test the next pattern list in the case statement.

Here's an example of how they might be used:

```bash
case "$variable" in
  pattern1)
    command1
    ;;
  pattern2)
    command2
    ;&
  pattern3)
    command3
    ;;&
  *)
    default_command
    ;;
esac
```

## Control Operators

A control operator in bash is one of those `||`, `&&`, `&`, `;`, `;;`, `;&`, `;;&`, `|`, `|&`, `(`, or `)`

These control operators do have precedence and associativity rules, similar to operators in programming languages. Here's a rough breakdown:

1. `&&` and `||` have the same precedence and are left-associative. They allow you to execute a command based on the success (`&&`) or failure (`||`) of the previous command.
2. `;` and `&` have the same precedence, which is lower than `&&` and `||`. They allow you to separate commands (`;`) or run a command in the background (`&`).
3. `|` and `|&` have higher precedence than `&&`, `||`, `;`, and `&`. They allow you to create pipelines, where the output of one command is used as the input of the next command (`|`), or where both the output and error output of one command are used as the input of the next command (`|&`).
4. `(` and `)` can be used to group commands, which can override the default precedence rules.
5. `;;`, `;&`, and `;;&` are used in the context of a case statement to separate different cases.

## Wildcards

For wildcard expansion, you would typically use the glob function, as I mentioned in the previous response. Here's how you can modify your code to expand wildcards in the input:

For example, if a user types `ls *.txt`, the shell should expand the `*.txt` wildcard to a list of all `.txt` files in the current directory.

```c
#include <glob.h>

// ...

char *input;
input = readline(" splash 💦 > ");
while (input != NULL)
{
    add_input_to_history(input);

    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));

    // Expand wildcards in the input
    if (glob(input, GLOB_TILDE, NULL, &glob_result) == 0) {
        for (int i = 0; i < glob_result.gl_pathc; ++i) {
            // Replace the input line with the expanded wildcard
            rl_replace_line(glob_result.gl_pathv[i], 0);
            // Redraw the input line
            rl_redisplay();
        }
    } else {
        ft_printf("You entered: %s\n", input);
    }

    globfree(&glob_result);
    input = readline(" splash 💦 > ");
}

free(input);
```