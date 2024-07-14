/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 19:33:34 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/09 16:28:16 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash_error.h"
#include "utils.h"
#include <libft.h>

/*
I need this to print on stderr and return 0
*/
bool	false_and_perr(const char *msg)
{
	perror(msg);
	return (false);
}

/*
I sometimes need to return NULL on error
*/
void	*null_on_err(const char *message)
{
	perror(message);
	return (NULL);
}

/*
Print error to standard error and return passed status.
the format defaults to "minishell: " followed by the 
argument or command like argv[0] and then the custom error message.
*/
uint8_t	print_error_status2(const char *message, const char *message2, uint8_t status)
{
	// ssize_t	result;

	if (!message)
	{
		if (!ft_write(2, "minishell error"))
			return (1);
	}
	if (message)
	{
		if (!ft_write(2, "minishell: ") || !ft_write(2, message))
			return (1);
	}
	// result = write(2, "minishell: ", 12);
	// if (result == -1 || result != 12) 
	// 	status = status_and_perror("write", 1);
	// result = write(2, message, ft_strlen(message));
	// if (result == -1 || result != (ssize_t)ft_strlen(message)) 
		// status = status_and_perror("write", 1);
	if (message2)
	{
		if (!ft_write(2, message2))
			return (1);
	}
	return (status);

	// result = write(2, message2, ft_strlen(message2));
	// if (result == -1 || result != (ssize_t)ft_strlen(message2)) 
	// 	status = status_and_perror("write", 1);
	// result = write(2, "\n", 1);
	// if (result == -1 || result != 1) 
	// 	status = status_and_perror("write", 1);
	// return (status);
}

/*
Print error to standard error and return passed status.
*/
uint8_t	print_error_status(const char *message, uint8_t status)
{
	// ssize_t	result;
// 
	if (message)
	{
		if (!ft_write(2, message) || !ft_write(2, "\n"))
			return (1);
		// result = write(2, message, ft_strlen(message));
		// if (result == -1 || result != (ssize_t)ft_strlen(message)) 
		// 	status = status_and_perror("write", 1);
		// result = write(2, "\n", 1);
		// if (result == -1 || result != 1) 
		// 	status = status_and_perror("write", 1);
	}
	else
	{
		if(!ft_write(2, "minishell error"))
			return (1);
	}
	return (status);
}

/*
Print minishell error to standard error and return status.
*/
uint8_t	print_minishell_error_status(const char *message, uint8_t status)
{
	ssize_t	result;

	result = 0;
	if (message)
	{
		if (!ft_write(2, "minishell: ") || !ft_write(2, message))
			return (1);
	}
	// if (message)
	// {
	// 	result = write(2, "minishell: ", 12);
	// 	if (result == -1 || result != 12) 
	// 		status = status_and_perror("write", 1);
	// 	result = write(2, message, ft_strlen(message));
	// 	if (result == -1 || result != (ssize_t)ft_strlen(message)) 
	// 		status = status_and_perror("write", 1);
	// }
	else
	{
		if (!ft_write(2, "minishell error"))
			return (1);
		// result = write(2, "minishell", 10);
		// if (result == -1 || result != 10) 
		// 	status = status_and_perror("write", 1);
	}
	if (!ft_write(2, "\n"))
		return (1);
	// result = write(2, "\n", 1);
	// if (result == -1 || result != 1) 
	// 	status = status_and_perror("write", 1);
	return (status);
}

/*
Write to fd and update status in case of write failure.
*/
bool	write_data(int fd, const void *str, uint8_t *status)
{
	ssize_t	result;

	result = write(fd, str, ft_strlen(str));
	if (result == -1 || result != (ssize_t)ft_strlen(str))
	{
		perror("write");
		*status = EXIT_FAILURE;
		return (false);
	}
	return (true);
}
