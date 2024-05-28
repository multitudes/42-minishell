/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:22:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 13:01:05 by lbrusa           ###   ########.fr       */
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
int	error_set_status(char *msg, int status)
{
	write(2, msg, ft_strlen(msg));
	g_exit_status = status;
	return (status);
}

/*
will exit the program with an error message
*/
int	exit_err_failure(char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(EXIT_FAILURE);
}
