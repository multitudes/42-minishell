/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:23:43 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 18:30:23 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "darray.h"

int	g_signal;

/*
The environ variable is part of the POSIX standard, so it should be 
available on any POSIX-compliant system.
according to the linux programming language by kerrisk (page 127), using
the environ variable is better than getting it in main.. (not posix compliant)
*/
extern char **environ;

// void free_ast(t_ast_node *node) 
// {
//     if (node == NULL) {
//         return;
//     }
//     // Free the left and right children
//     free_ast(node->left);
//     free_ast(node->right);

//     // Free the token list
//     t_list *current = node->token_list;
//     while (current != NULL) {
//         t_list *next = current->next;

//         // Free the token
//         t_token *token = (t_token *)current->content;
//         free(token->lexeme);  // Free the lexeme string
//         free(token);  // Free the token itself

//         // Free the list node
//         free(current);

//         current = next;
//     }

//     // Free the AST node itself
//     free(node);
// }

/*
util function
it makes sense to collect what we malloc in our data struct
because it is easier to free at the end or when we need to exit
token_list is freed in the loop with the ast
*/
void	free_data(t_data **data)
{
	if (data == NULL || *data == NULL)
		return ;
	debug("freeing env darray");
	darray_clear_destroy((*data)->env_arr);
	debug("freeing ast");
	free_ast(&(*data)->ast);
	free(*data);
	debug("data freed");
	*data = NULL;
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
		// return (0);
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

	while (environ && environ[i])
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
	debug("added %d env vars to my minishell data\n", env_array->end);
	*data = malloc(sizeof(t_data));
	if (*data == NULL)
	{
		perror("malloc init_data");
		darray_clear_destroy(env_array);
		return (0);
	}
	(*data)->env_arr = env_array;
	(*data)->ast = NULL;
	(*data)->exit_status = 0;
	(*data)->input = "";
	(*data)->token_list = NULL;
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
        rl_replace_line("", 0);
        rl_redisplay();
    }

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
but works better on the mac... 
also removed these couple lines... This was for testing
	// else if (isatty(STDIN_FILENO) == 0)
	// 	debug("non interactive mode \n");
*/
int	set_up_signals(void)
{
	if (isatty(STDIN_FILENO) == -1)
		return (error_set_status("is atty failed\n", 1));
	else if (isatty(STDIN_FILENO))
	{
		if ((signal(SIGINT, exit_handler) == SIG_ERR) || \
		(signal(SIGQUIT, SIG_IGN) == SIG_ERR))
			return (error_set_status("SIG_ERR signal failed\n", 1));
	}
	rl_catch_signals = 0;
	return (0);
}

void	exit_minishell(t_data *data)
{
	debug("exit_minishell");
	debug("freeing data");
	free_data(&data);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	write(1, "exit\n", 6);
}

void	update_env_exit_status_with(int exit_status, t_data *data)
{
	g_signal = exit_status;
	debug("final ast exit status: %d", g_signal);
	debug("Set env exit status to %d", g_signal);
	update_env(data, "?", ft_itoa(g_signal));
}

/*
set_up_signals() returns 1 but if it fails I leave it failing 
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
	t_data *data;

	data = NULL;
	if (!init_data(&data))
		return (1);
	load_history();
	set_up_signals();		
	while (data->input != NULL)
	{
		data->input = readline("minishell $ ");
		if (data->input && ft_strncmp(data->input, "", 1))
		{
			handle_history(data);
			data->token_list = tokenizer(data->input);
			if (data->token_list != NULL)
			{
				data->ast = create_ast(data->token_list);
				if (data->ast)
				{
					analyse_expand(data->ast, data);
					update_env_exit_status_with(execute_ast(data->ast, data), data);
					free_ast(&(data->ast));
				}
				else
					debug("syntax parse error");
			}
		}
		free((char *)(data->input));
	}
	exit_minishell(data);
	return (0); 
}
