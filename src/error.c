/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:22:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/03 16:10:01 by lbrusa           ###   ########.fr       */
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
	if (msg)
		perror(msg);
	else
		perror("");
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
	write(2, msg, ft_strlen(msg));
	return (0);
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

	result = write(2, message, ft_strlen(message));
	if (result == -1 || result != (ssize_t)ft_strlen(message)) 
		status = status_and_perror("write", 1);
	return (status);
}

/*
Print error and 
*/