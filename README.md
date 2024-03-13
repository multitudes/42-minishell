# 42-minishell
This project is about creating a simple shell

## authorised
### readline
The readline() function is not a standard C library function, but rather a function provided by the GNU Readline library. The GNU Readline library is used for reading lines of text with interactive features, such as line editing, history, and tab-completion.

The readline() function is used to read a line of text from the user, providing a simple interface for interactive command-line programs. Here's a basic example:
```
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

int main() {
    char *input;

    input = readline("Enter some text: ");

    if (input != NULL) {
        printf("You entered: %s\n", input);
        free(input); // Important to free the memory allocated by readline
    }

    return 0;
}

```
need linking with `-lreadline`





### rl_clear_history
The rl_clear_history function is part of the GNU Readline library, and it is used to clear the history list maintained by Readline. The history list typically stores previously entered command lines, allowing users to recall and edit them.

Here's a basic example demonstrating the use of rl_clear_history:
```
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    // Readline initialization
    rl_initialize();

    // Some example commands
    add_history("command1");
    add_history("command2");
    add_history("command3");

    // Display history before clearing
    printf("History before clearing:\n");
    for (int i = 0; i < history_length; i++) {
        printf("%d: %s\n", i + 1, history_get(i)->line);
    }

    // Clear history
    rl_clear_history();

    // Display history after clearing
    printf("\nHistory after clearing:\n");
    for (int i = 0; i < history_length; i++) {
        printf("%d: %s\n", i + 1, history_get(i)->line);
    }

    return 0;
}
```

rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs

## structure 
- Phases of the interpreter—scanning, parsing, and
evaluating code.

The first step is scanning, also known as lexing,
A scanner (or lexer) takes in the linear stream of characters and chunks them together into a series of something more akin to “words”. In programming languages, each of these words is called a token. Some tokens are single characters, like ( and , . Others may be several characters long, like numbers ( 123 ), string literals ( "hi!" ), and identifiers ( min )


The next step is parsing. This is where our syntax gets a grammar—the ability to compose larger expressions and statements out of smaller parts

A parser takes the flat sequence of tokens and builds a tree structure that mirrors the nested nature of the grammar.


## links
https://medium.com/@winfrednginakilonzo/guide-to-code-a-simple-shell-in-c-bd4a3a4c41cd


What is a shell?

A shell is a program that acts as an interface between the user and the operating system. A shell allows the user to interact with the operating system by accepting commands entered by the user from the keyboard, verifying if the inputs from the user are correct and executing them. Generally, a shell is a user interface that allows us to use computer resources such as memory. Think of a command-line interface such as Bash of Fish. 


Data streams: standard input, output and error

Interactive and non-interactive shells and scripts

One important aspect to consider when creating a simple shell is whether it is running in interactive mode or non-interactive mode.

In interactive mode, the shell is running in a command line interface and accepts input from the user. This is the mode that most users will be familiar with, and it is important to display a prompt to the user so that they know when the shell is ready to accept input.

In non-interactive mode, the shell is being used to run a script or a batch of commands, and does not accept input from the user. In this mode, there is no need to display a prompt, and the shell should exit once it has executed all the commands in the script.

To check whether the shell is running in interactive mode or non-interactive mode, we can use the isatty() function. This function checks whether a file descriptor refers to a terminal or not. If it returns true, then the shell is running in interactive mode and we should display a prompt. If it returns false, then the shell is running in non-interactive mode and we should not display a prompt.

Create an infinite loop for the prompt

Once we have determined whether the shell is running in interactive mode or non-interactive mode, we can display a prompt to the user.
Display prompt on interactive mode

Step 2: Design the Shell

The first step in designing your shell is to decide what features you want to include. Do you want your shell to have built-in commands, support for input/output redirection, pipes, or background jobs? Once you have a clear idea of the features you want to include, you can start designing the shell’s architecture.

Step 3: Read input from the user

The first step in making a shell is to read input from the user. This input can be a single command or multiple commands separated by a semicolon. To read input from the user, we can use the getline function which reads a line of input from the user. 

Step 4: Parse User Input

The next step is to parse the user’s input. You will need to read the input from the user and split it into tokens. These tokens can then be analyzed to determine the command the user wants to execute and any arguments or options they have provided. 

Step 5: Execute Commands

Once you have parsed the user’s input, you will need to execute the command. This involves creating a child process to run the command and using system calls to execute it. To execute the command, we can use the execve() function which executes a command by searching for the executable file in the system’s path environment variable. You will also need to handle any errors that may occur during command execution.

Step 6: Handle the PATH

A path is a list of directories that the shell uses to search for executable files. When a user enters a command in the shell, the shell searches for the corresponding executable file in each directory listed in the path until it finds the executable.

One way to handle paths using a linked list is to create a data structure that stores the directories in the path. The data structure can be implemented as a linked list, where each node in the list represents a directory in the path.

To create the linked list, you can start by parsing the path string and splitting it into its individual directories. Then, you can create a node for each directory and link them together using pointers.

Once you have the linked list of directories, you can use it to search for executable files when a user enters a command in the shell. 

Step 7: Implement Built-In Commands

Built-in commands are special commands that are implemented by the shell itself rather than being external programs. Some common built-in commands include cd, echo, and exit. You will need to implement these commands yourself. Built-in commands such as “cd” or “exit” which cannot be executed using the execve() function. To handle built-in commands, we can use if statements to check if the command is a built-in command by doing string comparison of the user input and the command and then execute the command accordingly.

Step 8: Support Input/Output Redirection

Input/output redirection allows the user to redirect the input or output of a command to a file instead of the screen or keyboard (standard input or output). To implement I/O redirection, we can use the dup2() function to redirect input or output to a file descriptor.
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

Step 9: Support Pipes

We can also implement pipes to allow the user to pipe the output of one command to the input of another command. To implement pipes, we can use the pipe() function to create a pipe and the fork() function to create a child process for each command.

5. **`|` (Pipe):**
   - Connects the output of one command as the input to another command.
   - Example: `ls | grep "pattern"`
   - This command lists the files in the current directory and pipes the output to `grep` to filter lines containing the specified pattern.

this is not needed in our minishell but leaving it here for completeness.
6. **`2>` and `2>>` (Standard Error Redirection):**
   - Redirects the standard error output of a command to a file (similar to `>` and `>>` for standard output).
   - Example: `command 2> error.log`
   - This command runs `command` and redirects any error messages to a file named `error.log`.

These redirection operators provide a flexible way to manipulate the flow of data between commands and files in Bash scripts.

Step 10: Support Background Jobs

Background jobs allow a command to be executed in the background, allowing the user to continue working in the shell while the command runs. 

Step 11: Test the Shell

Once you have implemented all the features you want, you should create tests for your shell 

ERROR HANDLING

One important aspect of error handling in a shell is checking the return value of system calls and library functions. These calls may fail for various reasons, such as invalid input, insufficient memory, or resource constraints. For example, if the fork() system call fails, it may indicate that the system is out of process resources.

Another aspect of error handling is handling signals. Signals are used by the operating system to notify a process of various events, such as a segmentation fault or a user interrupt. 

Finally, a shell should provide meaningful error messages to the user when a command fails. For example, if the user enters an invalid command, the shell should display an error message indicating that the command is not recognized. Similarly, if a command fails due to a system error, the shell should display an appropriate error message to the user.

Examples of the types of errors that a simple shell may encounter and how they can be handled
ex
Incorrect command:

Incorrect number of arguments:

Permission denied:

System call error:

Memory Leaks

Memory leaks can occur when a program dynamically allocates memory but fails to free it, causing the program to consume more and more memory until it crashes or becomes unusable. Therefore, use valgrind to check whether your program has memory leaks, since you have to free() the memory allocations made with malloc(). Valgrind is a powerful tool that can detect memory leaks, as well as other common programming errors such as buffer overflows and null pointer dereferences. To use Valgrind, simply run your program with the “valgrind” command followed by the name of your executable.

valgrind ./myprogram

## bonus
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


## links
Compilers: Principles, Techniques, and Tools (universally known as “the Dragon Book”) is the canonical reference.

[Bash manual](https://www.gnu.org/software/bash/manual/bash.html)  
[Implement your own shell - GustavoRodriguez-RiveraandJustinEnnen](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
