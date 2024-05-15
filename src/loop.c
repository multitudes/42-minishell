/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:23:43 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/15 18:20:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "darray.h"

/*
The environ variable is part of the POSIX standard, so it should be 
available on any POSIX-compliant system.
according to the linux programming language by kerrisk (page 127), using
the environ variable is better than getting it in main.. (not posix compliant)
*/
extern char **environ;

/*
util function
it makes sense to collect what we malloc in our data struct
because it is easier to free at the end or when we need to exit
*/
void	free_data(t_data *data)
{
	if (data == NULL)
		return ;
	// free the environ
	free((void*)data->input);
	darray_clear_destroy(data->env_arr);
	free(data->ast);
	free(data);
}


/*
to move to utilities header
*/
char 	*add_newline(char *input);
bool	contains_heredoc(const char *input, char **here_delimiter);


/*
functions to be moved to utilities!

check if the input contains a << DLESS operator and change 
the prompt to indicate that it expects more...
*/
bool	contains_heredoc(const char *input, char **here_delimiter)
{
	int	i;
	int start;

	i = 0;
	while (input[i])
	{
		if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
		{
			i += 2;
			while (is_space(input[i]))
				i++;
			if (input[i] == '\0')
			{
				debug("DLESS delimiter is missing\n");
				here_delimiter = NULL;
				return (true);
			}
			start = i;
			while (!filename_delimiter(input[i]))
				i++;
			if (i == start)
				here_delimiter = NULL;
			else
				*here_delimiter = ft_substr(input, start, i - start);
			printf("DLESS delimiter: %s\n", *here_delimiter);
			return (true);
		}
		i++;	
	}
	return (false);
}

/*
in case the input contains a << DLESS operator, we need to check if the
input contains the delimiter string
if not we need to keep reading the input until we find it
*/
bool contains_string(char *here_content, char *DLESS_delimiter)
{
	// the DLESS can be any string
	int i = 0;
	int j = 0;
	int k = 0;
	while (here_content[i])
	{
		if (here_content[i] == DLESS_delimiter[0])
		{
			k = i;
			while (here_content[k] && DLESS_delimiter[j] && here_content[k] == DLESS_delimiter[j])
			{
				// debug("here_content[k] == DLESS_delimiter[j] %c %c\n", here_content[k], DLESS_delimiter[j]);	
				k++;
				j++;
			}
			if ((int)ft_strlen(DLESS_delimiter) == j)
			{
				debug("they are equal\n");
				return (true);
			}	
		}
		j = 0;
		i++;
	}
	return (false);
}

/*
used for a DLESS test but not implemented yet
*/
char *add_newline(char *input)
{
	char *here_content;

	here_content = ft_strjoin(input, "\n");
	free(input);
	return (here_content);
}


/*
initialiser for data
*/
int	init_data(t_data **data)
{
	size_t i;
	char *env;
	t_darray *env_array;

	// check if environ is NULL
	if (environ == NULL)
	{
		perror("environ is NULL");
		return (0);
	}
	i = 0;
	
	// save the envp - start with a array of with el of size 100 which will be expanded
	// if needed
	env_array = darray_create(sizeof(char *), 100);
	if (env_array == NULL)
	{
		perror("malloc env_array");
		return (0);
	}
	i = 0;
	while (environ[i])
	{ 

		env = ft_strdup(environ[i]);
		if (env == NULL)
		{
			perror("malloc env");
			darray_clear_destroy(env_array);
			return (0);
		}
		if (darray_push(env_array, env) == -1)
		{
			perror("malloc darray_push");
			darray_clear_destroy(env_array);
			return (0);
		}
		i++;
	}
	darray_push(env_array, NULL);
	debug("env_array->end: %d\n", env_array->end);
	*data = malloc(sizeof(t_data));
	if (*data == NULL)
	{
		perror("malloc init_data");
		return (0);
	}
	(*data)->env_arr = env_array;
	(*data)->ast = NULL;
	(*data)->DLESS_delimiter = NULL;
	(*data)->exit_status = 0;
	(*data)->input = NULL;
	(*data)->token_list = NULL;
	return (1);
}

/*
Not a handler - it will exit if there is an error
*/
int	_exit_err(char *msg)
{
	write(1, msg, ft_strlen(msg));
	return (1);
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
static void	exit_handler(int sig)
{
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_redisplay();
        rl_replace_line("", 0);
    }
	return ;
}



/*
	// check if interactive or not example
	// the isatty() function returns 1 if the file descriptor is an open file,
	// 0 if it is not, and -1 if an error occurs.
	if (isatty(STDIN_FILENO))
		debug("interactive mode \n");
	else if (isatty(STDIN_FILENO) == 0)
		debug("non interactive mode \n");
	else if (isatty(STDIN_FILENO) == -1)
		perror("isatty\n");

	TODO:
	init and implement signals. ctrl-c, ctrl-d, ctrl-z, ctrl-\
	create error handling - check if input is valid - Incorrect command -
		incorrect number of arguments - permission denied - system call error
*/
int loop(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	t_data *data;

	data = NULL;
	// init data structure with environ and path
	if (!init_data(&data))
		return (1);
	load_history();

	// on mac I with ctrl - c i get a new line on bash
	rl_catch_signals = 0;

	// signal handling
	// ◦ ctrl-C SIGINT displays a new prompt on a new line. 
	// ◦ ctrl-D exits the shell.
	// ◦ ctrl-\ SIGQUIT does nothing.	
	
	if (isatty(STDIN_FILENO))
	{
		if ((signal(SIGINT, exit_handler) == SIG_ERR) || \
		(signal(SIGQUIT, SIG_IGN) == SIG_ERR))
		return (_exit_err("SIG_ERR signal failed\n"));
		debug("interactive mode \n");
	}
	else if (isatty(STDIN_FILENO) == 0)
		debug("non interactive mode \n");
	else if (isatty(STDIN_FILENO) == -1)
		perror("isatty\n");
	// The sig argument identifies the signal whose disposition 
	// we want to retrieve or change. 
	//This argument can be any signal except SIGKILL or SIGSTOP.

	
	// signal(SIGINT, SIG_IGN);
	
	data->input = "";
	while (data->input != NULL)
	{
		data->input = readline("minishell $ ");
		
		if (data->input && ft_strncmp(data->input, "", 1))
		{
			sanitize_input(data->input);
			// check best error handling
			if (!handle_history(data))
				debug("failed to handle history\n");
			t_list *token_list = tokenizer(data->input);
			if (token_list != NULL)
			{
				/*
				since the create_ast is recursive I pass the token list
				separately to avoid rewriting the copy in my data 
				*/
				data->ast = create_ast(data, token_list);
				if (data->ast)
				{
					print_ast(data->ast);
					analyse_expand(data->ast, data);
					execute_ast(data->ast, data);
				}
				else
					debug("syntax error");
			}
		}
	}
	/*
	The readLine() function, reads a line of input from
	the user on the command line and returns the result.
	To kill an interactive command-line app, type Control-D.
	Doing so signals an “end-of-file” condition to the program.
	When that happens readLine() returns null ,
	so we check for that to exit the loop.
	*/

//	debug("freeing ===================");
	free_data(data);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	write(1, "exit\n", 6);
	return (0);
}