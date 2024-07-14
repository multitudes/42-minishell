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
#include "utils.h"
#include <stdint.h>
#include <libft.h>
#include <stdbool.h>

/*
Prepends "minishell: " to msg before calling perror.
*/
void	perror_minishell(const char *msg)
{
	char	*perror_msg;

	if (msg)
		perror_msg = ft_strjoin("minishell: ", msg);
	else
		perror_msg = ft_strdup("minishell error");
	perror(perror_msg);
	free(perror_msg);
}

/*
Write to perror in the format "minishell: msg" with msg and
bool return value passed as arguments.
*/
bool	perror_and_bool(const char *msg, bool return_value)
{
	char	*perror_msg;

	if (msg)
		perror_msg = ft_strjoin("minishell: ", msg);
	else
		perror_msg = ft_strdup("minishell error");
	perror(perror_msg);
	free(perror_msg);
	return (return_value);
}

/*
Write to perror in the format "minishell: msg" with msg and
status return value passed as arguments.
*/
uint8_t	perror_and_status(const char *msg, uint8_t status)
{
	char	*perror_msg;

	if (msg)
		perror_msg = ft_strjoin("minishell: ", msg);
	else
		perror_msg = ft_strdup("minishell error");
	perror(perror_msg);
	free(perror_msg);
	return (status);
}

/*
Joins two consecutive messages and passes the new message to perror,
frees the joined string, and return the status
TODO change a variadic function
*/
uint8_t	perror_and_status2(const char *msg_1, const char *msg_2, uint8_t status)
{
	char	*perror_msg;

	if (msg_1 || msg_2)
		perror_msg = ft_strjoin3("minishell: ", msg_1, msg_2);
	else
		perror_msg = ft_strdup("minishell error");
	perror(perror_msg);
	free(perror_msg);
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
	// ssize_t	result;

	// result = 0;
	if (msg)
		ft_write(2, msg);
	else
		ft_write(2, "minishell: error");
	ft_write(2, "\n");
	// {
	// 	write(2, msg, ft_strlen(msg));
	// 	if (result == -1 || result != (ssize_t)ft_strlen(msg)) 
	// 		perror("write");
	// 	result = write(2, "\n", 1);
	// 	if (result == -1 || result != 1) 
	// 		perror("write");
	// }
	return (0);
}

/*
I need this to print on stderr and return 0
*/
bool	false_and_print(const char *msg)
{
	// ssize_t	result;

	// result = 0;
	if (msg)
		ft_write(2, msg);
	else
		ft_write(2, "minishell: error");
	ft_write(2, "\n");
	// {
	// 	result = write(2, msg, ft_strlen(msg));
	// 	if (result == -1 || result != (ssize_t)ft_strlen(msg)) 
	// 		perror("write");
	// 	result = write(2, "\n", 1);
	// 	if (result == -1 || result != 1) 
	// 		perror("write");
	// }
	return (false);
}
