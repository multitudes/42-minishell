## Allowed functions

Here at 42 we are allowed to use the following functions for this project:

| Function | Description |
| -------- | ----------- |
| `execve()` | Executes a program specified by a filename. |
| `readline()` | Reads a line from the standard input and returns it as a string. |
| `rl_clear_history()` | Clears the history list maintained by the GNU Readline library. |	
| `rl_on_new_line()` | Tells the GNU Readline library that the cursor has moved to a new line. |
| `rl_replace_line()` | Replaces the current line with a new line. |
| `rl_redisplay()` | Updates the display to reflect the current contents of the line buffer. |
| `add_history()` | Adds a line to the history list maintained by the GNU Readline library. |
| `printf()` | Writes output to the standard output stream. |
| `malloc()`	| Allocates memory dynamically. |
| `free()` | Deallocates memory previously allocated by a call to malloc. |
| `write()` | Writes data to a file descriptor. |
| `access()` | Checks whether the calling process can access a file. |
| `open()` | Opens a file or device. |
| `read()` | Reads data from a file descriptor. |
| `close()` | Closes a file descriptor. |
| `fork()` | Creates a new process by duplicating the calling process. |
| `wait()` | Waits for a child process to terminate. |
| `waitpid()`  | Waits for a specific child process to terminate. |
| `wait3()` | Waits for any of the children process to terminate and returns resource usage information. |
| `wait4()` | Waits for a child process (passing its pid) to terminate and returns resource usage information. | 	
| `signal()` | Sets a signal handler for a specific signal. |
| `sigaction()` | Sets a signal handler for a specific signal. |
| `sigemptyset()` | Initializes an empty signal set. |
| `sigaddset()` | Adds a signal to a signal. |
| `kill()` | Sends a signal to a process. |
| `exit()` | Terminates the calling process. |
| `getcwd()` | Gets the current working directory. |
| `chdir()` | Changes the current working directory. |
| `stat()` | Gets file status. |
| `lstat()` | Gets file status. The file is a symbolic link. |
| `fstat()` | Gets status of the file pointed to from a file descriptor. |
| `unlink()` | Deletes a name from the filesystem. |
| `dup()` | creates a copy of the given file descriptor. The new descriptor is the lowest-numbered file descriptor not currently open for the proces |
| `dup2()` | Duplicates a file descriptor, but it takes an additional argument: the desired file descriptor number for the copy. |
| `pipe()` | Creates a pipe.  |
| `opendir()` | Opens a directory stream. |	
| `readdir()` | Reads a directory stream. |
| `closedir()` | Closes a directory stream. |
| `strerror()` | Returns a string describing an error code. |
| `perror()` | Prints a descriptive error message to the standard error stream. |
| `isatty()` | Checks whether a file descriptor refers to a terminal. |
| `ttyname()` | Returns the name of the terminal connected to a file descriptor. |
| `ttyslot()` | Returns the number of the slot in the utmp file associated with the current process. |
| `ioctl()` | Performs device-specific operations. |
| `getenv()` | Gets the value of an environment variable. |
| `tcsetattr()` | Sets the parameters associated with a terminal. |
| `tcgetattr()` | Gets the parameters associated with a terminal. |
| `tgetent()` | Gets the entry in the terminfo database. |
| `tgetflag()` | Gets the value of a terminfo flag. |
| `tgetnum()` | Gets the value of a terminfo number. |
| `tgetstr()` | Gets the value of a terminfo string. |
| `tgoto()` | Instantiates the output of a given capability with parameters. |
| `tputs()`	| Outputs a string to the terminal. |

Also we follow the NORM, a series of rules about linting and formatting of the code. Examples: functions cannot have more than 25 lines; we are not allowed to use "for"-loops, but while loops are allowed; declaring and defining variables in one line is not allowed. etc.

## Builtin functions to recreate
Builtin commands are special commands that are implemented in the shell itself rather than being external programs. Some common builtin commands include cd, echo, and exit. Builtin commands such as “cd” or “exit” cannot be executed using the `execve()` function. Our minishell shall have the following Builtins which we need to recreate:
◦ echo with option -n
◦ cd with only a relative or absolute path 
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options

Bash and other shalls have many more builtin commands.
