/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:02:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/02/25 15:17:37 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(void)
{
	/*
	TODO:
	init and implement signals. ctrl-c, ctrl-d, ctrl-z, ctrl-\
	create error handling - check if input is valid - Incorrect command -
		incorrect number of arguments - permission denied - system call error
	check if interactive or not - display prompt then
	use readline function to read input
	scan and tokenixe input - for a start, just echo the input
	parsing the tokens - different type for each keyword, operator, 
		bit of punctuation, and literal type.

	
	*/
	ft_printf("Hello, World!\n");

	// readline example
	char *input;
    input = readline("Enter some text: ");
    if (input != NULL) {
        printf("You entered: %s\n", input);
        free(input);
    }

	// check if interactive or not example
	if (isatty(STDIN_FILENO))
	{
		ft_printf("Interactive mode\n");
	}
	else
	{
		ft_printf("Non-interactive mode\n");
	}
		
	/*
	The readLine() function, reads a line of input from 
	the user on the command line and returns the result. 
	To kill an interactive command-line app, type Control-D. 
	Doing so signals an “end-of-file” condition to the program. 
	When that happens readLine() returns null , 
	so we check for that to exit the loop.
	*/	

	return (0);
}

/*
example testing
true && echo "This command always succeeds"
false || echo "This command always fails"

BUILTINS
the -n option prevents echo from adding a newline character
$ echo -n "Hello, "; echo "World "
Hello, World

cd with only a relative or absolute path - pwd with no options
$ pwd
/home/user/Documents
$ cd Downloads
$ pwd
/home/user/Documents/Downloads
$ pwd
/home/user/Documents
$ cd /var/log
$ pwd
/var/log

export with no options
$ export MY_VARIABLE="Hello, World!"
$ echo $MY_VARIABLE
Hello, World!

unset with no options - used to unset or remove variables or functions
$ MY_VARIABLE="Hello, World!"
$ echo $MY_VARIABLE
Hello, World!
$ unset MY_VARIABLE
$ echo $MY_VARIABLE

env with no options or arguments
prints the current environment variables. 
$ env
USER=john_doe
HOME=/home/john_doe
PATH=/usr/local/bin:/usr/bin:/bin

exit with no options
$ exit
#!/bin/bash
echo "This is a script."
exit

or > exit
exits minishell and returns to the terminal

IMPLEMENT REDIRECTIONS
Redirection operators in Bash are used to control the input and output of commands. They allow you to manipulate where a command reads its input from and where it writes its output to. Here are some common redirection operators:

1. **`>` (Output Redirection):**
   - Redirects the output of a command to a file.
   - Example: `ls > output.txt`
   - This command lists the contents of the current directory and writes the output to a file named `output.txt`.

2. **`<` (Input Redirection):**
   - Takes the input for a command from a file.
   - Example: `sort < input.txt`
   - This command sorts the lines from the file `input.txt`.

3. **`>>` (Append Output):**
   - Appends the output of a command to a file (instead of overwriting it).
   - Example: `echo "new line" >> output.txt`
   - This command appends the text "new line" to the end of the file `output.txt`.

4. **`<<` (Here Document):**
   - Allows you to pass multiple lines of input to a command.
   - Example:
     ```bash
     cat << EOF
     Line 1
     Line 2
     EOF
     ```
     This command uses a here document to pass multiple lines to the `cat` command.

5. **`|` (Pipe):**
   - Connects the output of one command as the input to another command.
   - Example: `ls | grep "pattern"`
   - This command lists the files in the current directory and pipes the output to `grep` to filter lines containing the specified pattern.

6. **`2>` and `2>>` (Standard Error Redirection):**
   - Redirects the standard error output of a command to a file (similar to `>` and `>>` for standard output).
   - Example: `command 2> error.log`
   - This command runs `command` and redirects any error messages to a file named `error.log`.

These redirection operators provide a flexible way to manipulate the flow of data between commands and files in Bash scripts.

*****
handle environment variables

Handle $? 


BONUS
Certainly! Here are examples of using `&&` (logical AND) and `||` (logical OR) with parentheses for priorities in Bash:

### Using `&&` (Logical AND):

1. **Basic `&&` Example:**
   ```bash
   command1 && command2
   ```
   - If `command1` succeeds (returns a zero exit status), then `command2` will be executed.

2. **Multiple Commands with `&&`:**
   ```bash
   (command1 && command2) && command3
   ```
   - `command1` and `command2` are executed in sequence. If both succeed, then `command3` is executed.

3. **Mixing `&&` and `||`:**
   ```bash
   (command1 && command2) || command3
   ```
   - If `command1` and `command2` succeed, then `command3` will not be executed. If either `command1` or `command2` fails, then `command3` will be executed.

### Using `||` (Logical OR):

1. **Basic `||` Example:**
   ```bash
   command1 || command2
   ```
   - If `command1` fails (returns a non-zero exit status), then `command2` will be executed.

2. **Multiple Commands with `||`:**
   ```bash
   (command1 || command2) || command3
   ```
   - `command1` is executed. If it fails, then `command2` is executed. If both fail, then `command3` is executed.

3. **Mixing `&&` and `||`:**
   ```bash
   (command1 || command2) && command3
   ```
   - If `command1` succeeds, then `command2` will not be executed, and `command3` will be executed. If `command1` fails, then `command2` is executed, and `command3` will not be executed.

These examples showcase the use of `&&` and `||` for logical operations in Bash, and the parentheses are used to control the order of execution.

and 
Certainly! Wildcards, such as `*`, are used for pattern matching in file names. Here are some examples of using wildcards in the current working directory:

1. **List all files in the current directory:**
   ```bash
   ls *
   ```
   - This command lists all files (and directories) in the current directory.

2. **Remove all text files in the current directory:**
   ```bash
   rm *.txt
   ```
   - This command removes all files with a `.txt` extension in the current directory.

3. **Copy all `.jpg` files to another directory:**
   ```bash
   cp *.jpg /path/to/destination/
   ```
   - This command copies all files with a `.jpg` extension to the specified destination directory.

4. **Count the lines in all `.log` files:**
   ```bash
   wc -l *.log
   ```
   - This command counts the number of lines in each file with a `.log` extension in the current directory.

5. **Grep for a specific pattern in all `.md` files:**
   ```bash
   grep "pattern" *.md
   ```
   - This command searches for the specified pattern in all files with a `.md` extension.

6. **Archive all `.txt` files into a tarball:**
   ```bash
   tar -cvf archive.tar *.txt
   ```
   - This command creates a tarball (`archive.tar`) containing all files with a `.txt` extension in the current directory.

These examples demonstrate how wildcards can be used in combination with various commands for file manipulation and processing in the current working directory.

-- handle $?

My apologies for the confusion. You're correct. The reason `echo $?` prints 0 is because it's executed in a separate shell instance.

In the command:

```bash
echo "Hello" | grep "Helli" | echo $?
```

The `echo $?` is not capturing the exit status of the `grep` command in the pipeline because each command in a pipeline runs in a subshell, and `$?` is capturing the exit status of the last command executed within its own subshell, which is the `echo $?` itself.

So, the `grep` command in the middle doesn't directly affect the exit status seen by the outer shell, and `$?` in the outer shell remains 0.

If you want to capture the exit status of the `grep` command, you can use an alternative approach, like assigning the exit status to a variable:

```bash
echo "Hello" | grep "Helli"; status=$?; echo $status
```

This way, the `$status` variable captures the exit status of the `grep` command, and `echo $status` will correctly print 1.
...

do we need to need to tell users where errors occurred. ?

*/