/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:23:43 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/05 14:22:43 by lbrusa           ###   ########.fr       */
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
void	free_data(t_mini_data *data)
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

check if the input contains a << heredoc operator and change 
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
				debug("heredoc delimiter is missing\n");
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
			printf("heredoc delimiter: %s\n", *here_delimiter);
			return (true);
		}
		i++;	
	}
	return (false);
}

/*
in case the input contains a << heredoc operator, we need to check if the
input contains the delimiter string
if not we need to keep reading the input until we find it
*/
bool contains_string(char *here_content, char *heredoc_delimiter)
{
	// the heredoc can be any string
	int i = 0;
	int j = 0;
	int k = 0;
	while (here_content[i])
	{
		if (here_content[i] == heredoc_delimiter[0])
		{
			k = i;
			while (here_content[k] && heredoc_delimiter[j] && here_content[k] == heredoc_delimiter[j])
			{
				// debug("here_content[k] == heredoc_delimiter[j] %c %c\n", here_content[k], heredoc_delimiter[j]);	
				k++;
				j++;
			}
			if ((int)ft_strlen(heredoc_delimiter) == j)
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
used for a heredoc test but not implemented yet
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
int	init_data(t_mini_data **data)
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
	*data = malloc(sizeof(t_mini_data));
	if (*data == NULL)
	{
		perror("malloc init_data");
		return (0);
	}
	(*data)->env_arr = env_array;
	(*data)->ast = NULL;
	(*data)->heredoc_delimiter = NULL;
	(*data)->exit_status = 0;
	(*data)->input = NULL;
	return (1);
}

/*
*/
int loop(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	// main data structure for the shell
	t_mini_data *data;

	data = NULL;
	// init data structure with environ and path
	if (!init_data(&data))
		return (1);

	// check if interactive or not. test...
	if (isatty(STDIN_FILENO))
		debug("interactive mode \n");
	else if (isatty(STDIN_FILENO) == 0)
		debug("non interactive mode \n");
	else if (isatty(STDIN_FILENO) == -1)
		perror("isatty\n");

	/*
	TODO:
	init and implement signals. ctrl-c, ctrl-d, ctrl-z, ctrl-\
	create error handling - check if input is valid - Incorrect command -
		incorrect number of arguments - permission denied - system call error
	*/
	
	// load the history from the file
	load_history();
	data->input = readline("minishell $ ");
	while (data->input != NULL)
	{
		sanitize_input(data->input);
		
		// check if the input contains a << heredoc operator and change 
		// the prompt to indicate that it expects more... however tis 
		// is probably not what we will do in the end because we need to 
		// make it working with pipes.
		// so keep it here for now..
		// if (contains_heredoc(input, &data->heredoc_delimiter))
		// {
		// 	if (data->heredoc_delimiter == NULL || ft_strncmp(data->heredoc_delimiter, "", 1) == 0)
		// 	{
		// 		// perror("malloc data->heredoc_delimiter");
		// 		free(input);
		// 	}
		// 	// add newline to my input
		// 	input = add_newline(input);
		// 	// read the next input prompt	
		// 	char *here_content;
		// 	while(1)
		// 	{
		// 		here_content = readline("> ");
		// 		here_content = add_newline(here_content);
		// 		debug("here_content: %s\n", here_content);
		// 		sanitize_input(here_content);
		// 		input = ft_strjoin(input, here_content);
		// 		if (contains_string(here_content, data->heredoc_delimiter))
		// 		{
		// 			debug("break\n");
		// 			debug("input: %s\n", input);
		// 			free(here_content);
		// 			break;
		// 		}
		// 		free(here_content);
		// 	}
		// }
		// else 
		if (ft_strncmp(data->input, "", 1) != 0)
		{
			// check best error handling
			if (!handle_history(data))
				debug("failed to handle history\n");
			// debug("You entered: %s\n", input);
			t_list *tokens = tokenizer(data);
			if (tokens != NULL)
			{
				t_ast_node *asttree = create_ast(data, tokens);
				if (asttree == NULL)
				{
					// TODO define error handling
					debug("failed to create_ast - parse error");
				}
				else
				{
					print_ast(asttree);
					data->ast = asttree;
					analyse_expand(data->ast, data);
					execute_ast(data->ast, data);
				}
			}
		}
		// read the next input (prompt
		data->input = readline("minishell $ ");
	}
	/*
	The readLine() function, reads a line of input from
	the user on the command line and returns the result.
	To kill an interactive command-line app, type Control-D.
	Doing so signals an “end-of-file” condition to the program.
	When that happens readLine() returns null ,
	so we check for that to exit the loop.
	*/
	// check if interactive or not example
	// the isatty() function returns 1 if the file descriptor is an open file,
	// 0 if it is not, and -1 if an error occurs.

	debug("freeing ===================");
	free_data(data);
	return (0);
}