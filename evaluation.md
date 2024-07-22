
# Evaluation

## Colors in the prompt
It turned out that having colors in the prompt is a bad idea because due to the ansi excape codes the shell can't calculate the length of the prompt correctly. This leads to the cursor being in the wrong place when you press the arrow keys in some extra long commands or when copying and pasting.

We had this problem in the minishell. Since we changed the prompt at the end of the project, we did not expect that this would break things or have unexxpected consequenses. But it did. We had some tests failing and we had no idea why. 

We discussed it with our peers and found out that the bash would have the same behaviour.  
Our readline prompt:

```bash
data->input = readline("\033[44m\033[97m splash 💦 \033[0m\033[44m\033[97m>\033[0m ");
```
which looks like this: 
<div>
	<img src="assets/prompt.png" alt="prompt" width="400"/>
</div>

we can let bash do the same thing with the following command:
```bash
PS1="\033[44m\033[97m splash 💦 \033[0m\033[44m\033[97m>\033[0m "
```
and it would also have the same behaviour.
There are some functions that we are not allowed to use for this project that would fix the problem. But for now we will just make the prompt go over two lines. Since the terminal gui is getting confused by the ansi escape codes and prompt length, we will just make the prompt go over two lines. This way the bottom prompt will not have colors and be unaffected.

```bash
data->input = readline("\033[44m\033[97m splash 💦 \033[0m\033[44m\033[97m\033[0m\n>");
```

Now it looks like this:
<div>
	<img src="assets/prompt2.png" alt="prompt" width="400"/>
</div>

## Bash ambiguous redirect
This is not a feature but in bash when you do this:

```bash
bash: A="one two"
bash: B="three four"
bash: echo hi>$A""''""$B
# you get:
bash: $A""''""$B: ambiguous redirect
```

In our minishell we actually create a file called "one twothree four" and write "hi" to it.  

```bash
export A="one two"
export B="three four"
echo hi>$A""''""$B
ls
```

This is good :) The reason for the bash behaviour is that it gives meaning to empty strings like `""` and `''`. We do not need to do it and out behaviour is expected.

## history of shells
It also good to know a bit of history ad how the shells have evolved.
See this page for some extra facts about the evolution of different shells. 
[https://www.in-ulm.de/~mascheck/bourne/](https://www.in-ulm.de/~mascheck/bourne/)

## look into your executables
So you compiled your minishell with makes. You have like 150 files into one, how to check if you used onluy nonn foridden functions?

```bash
nm -u ./minishell

# this is the output
_access
_add_history
_chdir
_close
_closedir
_dup
_dup2
_environ
_execve
_exit
_fork
_free
_getcwd
_getenv
_ioctl
_isatty
_malloc
_memcpy
_open
_opendir
_perror
_pipe
_printf
_read
_readdir
_readline
_rl_catch_signals
_rl_clear_history
_rl_on_new_line
_rl_redisplay
_rl_replace_line
_signal
_stat
_strcmp
_unlink
_waitpid
_write
```

The -u specifically looks for the functions which are `included`, so functions we get from c libraries and not the ones we created ourselves.

To know more about a specific function and where it is used you can use the following command:

```bash
grep -InR ioctl
```
The flags for `grep` in case you are curious are:
-I is an option that tells grep to ignore binary files.
-n is an option that tells grep to display line numbers along with the matching lines.
-R is an option that tells grep to search recursively in directories and subdirectories.

And since we are allowed to use one global variable only and for the signals only... of course our evaluator looks for it in in the repo with a simple command and why not use some color!

```bash
grep --color -InR '\<g_'
```

<div>
	<img src="assets/globals.jpg" alt="globals" width="400">
</div>

## The signals
in the minishell signals are implemented in three places. In the readline main loop where SIGINT will just redisplay the prompt and SIGQUIT will do nothing.
In the blocking child processes where SIGINT will kill the child and SIGQUIT will print the exit status of the child.
