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