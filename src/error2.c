/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 19:33:34 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/28 19:39:41 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
// #include <stdint.h>
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
*/
uint8_t	print_error_status(const char *message, uint8_t status)
{
	ssize_t	result;

	result = write(2, message, ft_strlen(message));
	if (result == -1 || result != (ssize_t)ft_strlen(message)) 
		status = status_and_perror("write", 1);
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
