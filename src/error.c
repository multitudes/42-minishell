/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:22:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/06 14:39:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash_error.h"
#include <stdint.h>
#include <libft.h>
#include <stdbool.h>

/*
Used to return from the program with an error message passing
the exit error code. It is used in the executer and the parser.
I dont need to pass the data struct because when used in the child 
process, any updates will be lost upon return. Therefore, I will
keep it simple and just pass the message and the status.
Updating the global makes sense when I am in the parent process.  
*/
uint8_t	status_and_perror(const char *msg, uint8_t status)
{
	perror(msg);
	return (status);
}

/*
Joins two consecutive messages and passes the new message to perror,
frees the joined string, and return the status
TODO change a variadic function
*/
uint8_t	status_perror2(const char *msg_1, const char *msg_2, uint8_t status)
{
	ssize_t	result;

	if (msg_1)
	{
		result = write(2, msg_1, ft_strlen(msg_1));
		if (result == -1 || result != (ssize_t)ft_strlen(msg_1))
			status = status_and_perror("write", 1);
	}
	if (msg_2)
	{
		result = write(2, msg_2, ft_strlen(msg_2));
		if (result == -1 || result != (ssize_t)ft_strlen(msg_2))
			status = status_and_perror("write", 1);
	}
	result = write(2, ": ", 2);
	if (result == -1 || result != 2)
		status = status_and_perror("write", 1);
	perror("");
	return (status);
}

/*
will exit the program with an error message
*/
uint8_t	exit_and_print_err(const char *msg, uint8_t status)
{
	perror(msg);
	exit(status);
}

/*
I need this to print on stderr and return 0
*/
uint8_t	zero_and_printerr(const char *msg)
{
	ssize_t	result;

	result = 0;
	if (msg)
	{
		write(2, msg, ft_strlen(msg));
		if (result == -1 || result != (ssize_t)ft_strlen(msg)) 
			perror("write");
		result = write(2, "\n", 1);
		if (result == -1 || result != 1) 
			perror("write");
	}
	return (0);
}

/*
I need this to print on stderr and return 0
*/
bool	false_and_print(const char *msg)
{
	ssize_t	result;

	result = 0;
	if (msg)
	{
		result = write(2, msg, ft_strlen(msg));
		if (result == -1 || result != (ssize_t)ft_strlen(msg)) 
			perror("write");
		result = write(2, "\n", 1);
		if (result == -1 || result != 1) 
			perror("write");
	}
	return (false);
}
