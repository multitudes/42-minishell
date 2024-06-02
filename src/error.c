/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:22:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 09:59:19 by lbrusa           ###   ########.fr       */
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
int	status_and_print(char *msg, int status)
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
I sometimes need to return NULL on error
*/
void	*return_null_on_err(char *message)
{
	perror(message);
	return (NULL);
}