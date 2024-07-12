/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:23:43 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/12 11:44:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "darray.h"
#include "splash_error.h"
#include "analyser.h"
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
util function
it makes sense to collect what we malloc in our data struct
because it is easier to free at the end or when we need to exit
tokenlist is freed in the loop with the ast
*/
void	free_data(t_data **data)
{
	if (data == NULL || *data == NULL)
		return ;
	debug("freeing env darray");
	darray_clear_destroy((*data)->env_arr);
	free(*data);
	*data = NULL;
	debug("data freed");
}

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
		return (status_and_perror("is atty failed", 1));
	else if (isatty(STDIN_FILENO))
	{
		if ((signal(SIGINT, sigint_handler) == SIG_ERR) || \
		(signal(SIGQUIT, SIG_IGN) == SIG_ERR))
		return (status_and_perror("SIG_ERR signal failed", 1));
	}
	else 
	{
		if ((signal(SIGINT, sigint_handler) == SIG_ERR) || \
		(signal(SIGQUIT, SIG_IGN) == SIG_ERR))
		return (status_and_perror("SIG_ERR signal failed", 1));
	}
	rl_catch_signals = 1;
	return (0);
}

void	exit_minishell(t_data *data)
{
	debug("exit_minishell");
	debug("freeing data");
	free_data(&data);
}

void	update_env_exit_status_with(uint8_t exit_status, t_data *data)
{
	if (!update_env(data->env_arr, "?", ft_itoa(exit_status)))
		print_error_status("update_env", 1);
	
}

bool exit_condition(t_data *data)
{
	if (data->input == NULL )
		return (true);
	return false;
}

/*
with each new instance of the shell,
the env variable SHLVL is increased by one.
*/
void	shlvl_init(t_data *data)
{
	char	*shlvl;

	shlvl = ft_itoa(ft_atoi(mini_get_env(data->env_arr, "SHLVL")) + 1);
	update_env(data->env_arr, "SHLVL", shlvl);
	free(shlvl);
}

/*
set_up_std_signals() returns 1 but if it fails I leave it failing 
silently or should we exit the program?
The readLine() function, reads a line of input from
the user on the command line and returns the result.
To kill an interactive command-line app, type Control-D.
Doing so signals an “end-of-file” condition to the program.
When that happens readLine() returns null ,
so we check for that to exit the loop.
*/
int loop()
{
	t_data	*data;
	uint8_t status;

	status = 0;
	data = NULL;
	if (!init_data(&data))
		return (EXIT_FAILURE);
	shlvl_init(data);
	save_fds(data);
	load_history(data->env_arr);
	set_up_std_signals(); //check return value / status?
	while (true)
	{
		data->input = readline("minishell $ ");
		if (!exit_condition(data))
		{
			if (ft_strncmp(data->input, "", 1) != 0)
			{
				handle_history(data);
				data->tokenlist = tokenizer(data->input);
				if (data->tokenlist != NULL)
				{
					data->ast = create_ast(data->tokenlist);
					if (data->ast)
					{
						data->exit_status = execute_ast(data->ast, data);
						restore_fds(data);
						debug("Exit status: %i", data->exit_status);
						free_ast(&(data->ast));
					}
					else
						debug("syntax parse error");
				}
				else
					data->exit_status = 2;
			}
			free((char *)(data->input));
			debug("Back to loop()");
			continue ;
		}
		debug("Break from loop()");
		break ;
	}
	status = data->exit_status;
	free((char *)(data->input));
	free_data(&data);

	debug("exit_minishell");
	debug("Exit status: %i", status);

	write(1, "exit\n", 5);
		
	return (status); 
}

/*
invoqued using the -c flag
*/
int single_command(const char *input)
{
	uint8_t	status;	
	t_data	*data;

	status = 0;
	data = NULL;
	if (!init_data(&data))
		return (EXIT_FAILURE);
	debug("single command init_data done");
	debug("input: %s", input);
	shlvl_init(data);
	save_fds(data);
	set_up_std_signals();		
	data->input = ft_strdup(input);
	if (!exit_condition(data))
	{
		sanitize_input(data->input);
		data->tokenlist = tokenizer(data->input);
		if (data->tokenlist != NULL)
		{
			data->ast = create_ast(data->tokenlist);
			if (data->ast)
			{
				data->exit_status = execute_ast(data->ast, data);
				debug("Exit status: %i", data->exit_status);
				free_ast(&(data->ast));
			}
			else
				debug("syntax parse error");
		}
		else
		{
			free((char *)(data->input));
			free_data(&data);
			exit(2);
		}
	}
	status = data->exit_status;
	free((char *)(data->input));
	free_data(&data);
	debug("exit_minishell");
	debug("Exit status: %i", status);

	return (status); 
}

void	save_fds(t_data *data)
{
	debug("Save for later restoration STDIN: %i, STDOUT: %i, STDERR: %i", STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stderr = dup(STDERR_FILENO);
	if (data->original_stdout < 0 || data->original_stdin < 0 || data->original_stderr < 0)
	{
		data->exit_status = 1;
		perror("minishell: dup");
	}
}

void	restore_fds(t_data *data)
{
	if (dup2(data->original_stdout, STDOUT_FILENO) < 0 || \
	dup2(data->original_stdin, STDIN_FILENO) < 0 || \
	dup2(data->original_stderr, STDERR_FILENO) < 0)
	{
		data->exit_status = 1;
		perror("minishell: dup2");
	}
	debug("Restored STDIN: %i, STDOUT: %i, STDERR: %i", STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
}
