/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:22:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/06 12:12:58 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

/*
Used to return from the program with an error message passing
the exit error code. It is used in the executer and the parser.
I dont need to pass the data struct because when used in the child 
process, any updates will be lost upon return. Therefore, I will
keep it simple and just pass the message and the status.
Updating the global makes sense when I am in the parent process.  
*/
int	status_and_perror(char *msg, int status)
{
	perror(msg);
	return (status);
}

/*
Joins two consecutive messages and passes the new message to perror,
frees the joined string, and return the status
*/
int status_and_strjoin_perror(char *msg_1, char *msg_2, int status)
{
	char	*perror_msg;

	perror_msg = ft_strjoin(msg_1, msg_2);
	perror(perror_msg);
	free(perror_msg);
	return (status);
}

/*
will exit the program with an error message
*/
int	exit_and_print_err(char *msg, int status)
{
	perror(msg);
	exit(status);
}

/*
I need this to print on stderr and return 0
*/
int zero_and_printerr(char *msg)
{
	ssize_t	result;

	result = 0;
	if (msg)
	{
		write(2, msg, ft_strlen(msg));
		if (result == -1 || result != (ssize_t)ft_strlen(msg)) 
			perror("write");
	}
	return (0);
}

/*
I need this to print on stderr and return 0
*/
bool false_and_print(char *msg)
{
	ssize_t	result;

	result = 0;
	if (msg)
	{
		result = write(2, msg, ft_strlen(msg));
		if (result == -1 || result != (ssize_t)ft_strlen(msg)) 
			perror("write");
	}
	return (false);
}

/*
I need this to print on stderr and return 0
*/
bool false_and_perr(char *msg)
{
	perror(msg);
	return (false);
}

/*
I sometimes need to return NULL on error
*/
void	*null_on_err(char *message)
{
	perror(message);
	return (NULL);
}

/*
Print error to standard error and return passed status.
*/
int	print_error_status(char *message, int status)
{
	ssize_t	result;

	result = 0;
	result = write(2, message, ft_strlen(message));
	if (result == -1 || result != (ssize_t)ft_strlen(message)) 
		status = status_and_perror("write", 1);
	return (status);
}

/*
Print minishell error to standard error and return status.
*/
int	print_minishell_error_status(char *message, int status)
{
	ssize_t	result;

	result = 0;
	if (message)
	{
		result = write(2, "minishell: ", 12);
		if (result == -1 || result != 12) 
			status = status_and_perror("write", 1);
		result = write(2, message, ft_strlen(message));
		if (result == -1 || result != (ssize_t)ft_strlen(message)) 
			status = status_and_perror("write", 1);
	}
	else
	{
		result = write(2, "minishell", 10);
		if (result == -1 || result != 10) 
			status = status_and_perror("write", 1);
	}
	result = write(2, "\n", 2);
	if (result == -1 || result != 2) 
		status = status_and_perror("write", 1);
	return (status);
}