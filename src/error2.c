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
uint8_t	print_error_status2(const char *message, \
							const char *message2, uint8_t status)
{
	if (!message && !message2 && !ft_write(2, "minishell error"))
			return (1);
	if (!ft_write(2, "minishell: "))
			return (1);
	if (message && !ft_write(2, message))
			return (1);
	if (message2 && !ft_write(2, message2))
			return (1);
	if (!ft_write(2, "\n"))
		return (1);
	return (status);
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
		// 	status = perror_and_status("write", 1);
		// result = write(2, "\n", 1);
		// if (result == -1 || result != 1) 
		// 	status = perror_and_status("write", 1);
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
	// ssize_t	result;

	// result = 0;
	if (message)
	{
		if (!ft_write(2, "minishell: ") || !ft_write(2, message))
			return (1);
	}
	// if (message)
	// {
	// 	result = write(2, "minishell: ", 12);
	// 	if (result == -1 || result != 12) 
	// 		status = perror_and_status("write", 1);
	// 	result = write(2, message, ft_strlen(message));
	// 	if (result == -1 || result != (ssize_t)ft_strlen(message)) 
	// 		status = perror_and_status("write", 1);
	// }
	else
	{
		if (!ft_write(2, "minishell error"))
			return (1);
		// result = write(2, "minishell", 10);
		// if (result == -1 || result != 10) 
		// 	status = perror_and_status("write", 1);
	}
	if (!ft_write(2, "\n"))
		return (1);
	// result = write(2, "\n", 1);
	// if (result == -1 || result != 1) 
	// 	status = perror_and_status("write", 1);
	return (status);
}

/*
I need this to print on stderr and return 0
*/
uint8_t	zero_and_printerr(const char *msg)
{
	if (msg)
		ft_write(2, msg);
	else
		ft_write(2, "minishell: error");
	ft_write(2, "\n");
	return (0);
}

/*
Write "minishell: msg" to standard error and return boolian.
If msg is NULL, "minishell: error is printed."
If write fails false is returned.
*/
bool	stderr_and_bool(const char *msg, bool boolian)
{
	char	*error_msg;

	if (msg)
		error_msg = ft_strjoin("minishell: ", msg);
	else
		error_msg = ft_strdup("minishell error");
	if (!ft_write(2, error_msg) || !ft_write(2, "\n"))
		boolian = false;
	free(error_msg);
	return (boolian);
}