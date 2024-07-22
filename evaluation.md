
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

## file descriptors
In valgrind there is a flag to check the open file descriptors. This is useful to check if you are closing all the file descriptors you open. Also they usually dont add up like memory leaks but still in a process we can open only 1000 of them. It is a good thing to double check the usage on exit.
We use valgrind (on Linux only) to check for file descriptors. The flag is `--track-fds=yes`.
```bash
valgrind --track-fds=yes ./minishell
```
but since the readline has so many errors due to "unreachable" memory we managed a different incantation:
```bash
echo $'echo hi\nexit' | valgrind --track-fds=yes ./minishell
```
Bash interprets `echo $'echo hi\nexit'` as a 2 liner. So effectively sending `echo hi` and `exit` to our minishell. And valgrind will check after exiting how many file descriptors are open.

## quotes expansion

This should just do `ls -l`:  
```bash
"/bin/ls" "-l"''""''""
```
and so on, these are just `ls`:    
```bash
'/bin/ls'
'/bin/'""'ls'
'/bin/'""'ls'$EMPTY
'/bin/'""'ls'$EMPTY$AGAIN
```

This will give `$$/bin/ls: No such file or directory`
```bash
$$'/bin/'""'ls'$EMPTY$AGAIN
```
but this will work:  
```bash
$EMPTY'/bin/'""'ls'$EMPTY$AGAIN
```

Other tests:
```bash
echo '$USER'
# prints $USER
echo "$USER"
# prints the username

echo h^C
# redisplays the prompt
echo 'hey ' / cat -e
# checks for maintaining spaces in the single quotes
```

## the env builtin
In the subject it says we need to implement the env builtin. In bash `env` is a program.
So this makes sure we are not just executing the program `env` but also implementing the builtin.
```bash
env -i 
# prints too many args
/bin/env -i
# prints nothing
```

## export builtin
the identifiers can start with _ or a letter and can contain _ or a letter or a digit.  
```bash
export _A='"what"'
export A_A='"'
echo $A_A # prints "
