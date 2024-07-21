/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:23:43 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/21 11:57:33 by lbrusa           ###   ########.fr       */
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
#include "utils.h"

/*
 * central function of the loop.c file
*/
void	tokenize_and_parse(t_data *data)
{
	handle_history(data->input);
	data->tokenlist = tokenizer(data->input);
	if (syntax_check_and_heredoc(data) && data->tokenlist != NULL)
	{
		data->ast = create_ast(data->tokenlist);
		if (data->ast)
		{
			data->exit_status = execute_ast(data->ast, data);
			free_ast(&data->ast);
		}
		else
			data->exit_status = stderr_and_status("syntax parse error", 1);
	}
	else
		ft_lstclear(&data->tokenlist, free_tokennode);
}

void	mainloop(t_data *data)
{
	while (true)
	{
		if (isatty(STDIN_FILENO))
			data->input = readline(\
				"\033[44m\033[97m splash 💦 \033[0m\033[44m\033[97m>\033[0m ");
		else
			data->input = get_next_line(STDIN_FILENO);
		if (data->input != NULL)
		{
			if (g_signal)
				data->exit_status = g_signal + 128;
			if (ft_strncmp(data->input, "", 1) != 0)
				tokenize_and_parse(data);
			free((char *)(data->input));
			g_signal = 0;
			continue ;
		}
		return ;
	}
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
int	loop(void)
{
	t_data	*data;
	uint8_t	status;

	status = 0;
	data = NULL;
	if (!init_data(&data) || set_up_std_signals() > 0)
		return (EXIT_FAILURE);
	load_history();
	mainloop(data);
	status = data->exit_status;
	free((char *)(data->input));
	free_data(&data);
	ft_write(1, "exit\n");
	return (status); 
}

/*

*/
void	single_command_innerloop(t_data *data)
{
	sanitize_input(data->input);
	data->tokenlist = tokenizer(data->input);
	if (syntax_check_and_heredoc(data) && data->tokenlist != NULL)
	{
		data->ast = create_ast(data->tokenlist);
		if (data->ast)
		{
			data->exit_status = execute_ast(data->ast, data);
			free_ast(&(data->ast));
		}
		else
			data->exit_status = stderr_and_status("syntax parse error", 2);
	}
	else
	{
		ft_lstclear(&(data->tokenlist), free_tokennode);
		free((char *)(data->input));
		free_data(&data);
		exit(2);
	}
}

/*
invoqued using the -c flag
*/
int	single_command_loop(const char *input)
{
	uint8_t	status;	
	t_data	*data;

	status = 0;
	data = NULL;
	if (!init_data(&data) && set_up_std_signals() > 0)
		return (EXIT_FAILURE);
	data->input = ft_strdup(input);
	if (data->input != NULL)
		single_command_innerloop(data);
	if (g_signal)
		status = g_signal + 128;
	else
		status = data->exit_status;
	free((char *)(data->input));
	free_data(&data);
	return (status); 
}
