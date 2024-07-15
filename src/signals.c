/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:10:12 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 12:33:59 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "darray.h"
#include "splash_error.h"
#include "heredoc.h"
#include "analyser.h"
#include <signal.h>
#include <fcntl.h>
#include "init.h"

/*
Allowed global variable for signals only. 
will record the signal number which has been received
We should check first if using WTERMSIG(status) is enough
otherwise we might need this. WTERMSIG(status) returns the signal number
that caused the child to terminate when the child did not
exit normally and it has been added to the executor.
*/
int	g_signal;

/*
The environ variable is part of the POSIX standard, so it should be 
available on any POSIX-compliant system.
according to the linux programming language by kerrisk (page 127), using
the environ variable is better than getting it in main.. (not posix compliant)
*/
extern char **environ;

/*
for signals
The readline library maintains an internal buffer of the current 
line being edited. 
This buffer is separate from what's displayed on the terminal.
When a signal like SIGINT is received, you might want to clear this 
buffer so that the user starts with a fresh line after the signal 
is handled.
Here's what each function does:
rl_on_new_line(): This function tells readline that the cursor is 
on a new line, so it should not try to clear the current line 
when redisplaying the prompt.
rl_replace_line("", 0): This function replaces the contents of 
readline's internal buffer with an empty string. The 0 argument 
means that the cursor should be placed at the start of the line.
rl_redisplay(): This function updates the display to match 
the current contents of the line buffer. Since you've just 
cleared the line buffer, this will effectively clear the line on 
the terminal.
*/
static void	sigint_handler(int sig)
{
    if (sig == SIGINT)
    {	
		// g_signal = sig;
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
		// write(1,"got signal\n",12);
    }
	// else
	// 	g_signal = sig;
	return ;
}

/*
on mac with ctrl - c , I get a new line on bash without 
displaying ^C
signal handling:
◦ ctrl-C SIGINT displays a new prompt on a new line. 
◦ ctrl-D exits the shell.
◦ ctrl-\ SIGQUIT does nothing.	
this line is maybe not needed rl_catch_signals = 0;
but works better on the mac... producing result closer 
to the bash shell mac version
all other signals are sent to the handler so we update the global
variable with the number of the signal received but 
usually only SIGINT is handled. the value of sigint is 2 
whichy is added to 128 and gives 130, the exit code for ctrl-c
*/
int	set_up_std_signals(void)
{
	if (isatty(STDIN_FILENO) == -1)
		return (perror_and_status("is atty", 1));
	else if (isatty(STDIN_FILENO))
	{
		if ((signal(SIGINT, sigint_handler) == SIG_ERR) || \
		(signal(SIGQUIT, SIG_IGN) == SIG_ERR))
		return (perror_and_status("SIG_ERR", 1));
	}
	else 
	{
		if ((signal(SIGINT, sigint_handler) == SIG_ERR) || \
		(signal(SIGQUIT, SIG_IGN) == SIG_ERR))
		return (perror_and_status("SIG_ERR signal", 1));
	}
	rl_catch_signals = 1;
	return (0);
}
