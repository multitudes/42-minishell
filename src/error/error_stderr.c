/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_stderr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 19:33:34 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 13:06:35 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash_error.h"
#include "utils.h"
#include <libft.h>

/*
Writes "minishell: msg\n" to standard error and returns status.
If msg is NULL, "minishell: error\n" is printed.
*/
uint8_t	stderr_and_status(const char *msg, uint8_t status)
{
	if (msg)
	{
		if (!ft_write(2, "minishell: ") || !ft_write(2, msg) \
				|| !ft_write(2, "\n"))
			return (1);
	}
	else
	{
		if (!ft_write(2, "minishell error"))
			return (1);
	}
	return (status);
}

/*
Writes "minishell: msg_1: msg_2\n" to standard error and returns status.
If msgs are NULL, "minishell: error\n" is printed.
*/
uint8_t	stderr_and_status2(const char *msg_1, \
							const char *msg_2, uint8_t status)
{
	if (!msg_1 && !msg_2 && !ft_write(2, "minishell error"))
		return (1);
	if (!ft_write(2, "minishell: "))
		return (1);
	if (msg_1 && !ft_write(2, msg_1))
		return (1);
	if (msg_2 && !ft_write(2, msg_2))
		return (1);
	if (!ft_write(2, "\n"))
		return (1);
	return (status);
}

/*
Writes "minishell: msg_1: msg_2: msg_3\n" to standard error and returns status.
If msgs are NULL, "minishell: error\n" is printed.
*/
uint8_t	stderr_and_status3(const char *msg_1, \
							const char *msg_2, const char *msg_3, \
							uint8_t status)
{
	if (!msg_1 && !msg_2 && !ft_write(2, "minishell error"))
		return (1);
	if (!ft_write(2, "minishell: "))
		return (1);
	if (msg_1 && !ft_write(2, msg_1))
		return (1);
	if (msg_2 && !ft_write(2, msg_2))
		return (1);
	if (msg_3 && !ft_write(2, msg_3))
		return (1);
	if (!ft_write(2, "\n"))
		return (1);
	return (status);
}

/*
Writes "minishell: msg\n" to standard error and returns boolean.
If msg is NULL, "minishell: error\n" is printed.
If write fails false is returned.
*/
bool	stderr_and_bool(const char *msg, bool boolean)
{
	char	*error_msg;

	if (msg)
		error_msg = ft_strjoin("minishell: ", msg);
	else
		error_msg = ft_strdup("minishell error");
	if (!ft_write(2, error_msg) || !ft_write(2, "\n"))
		boolean = false;
	free(error_msg);
	return (boolean);
}
