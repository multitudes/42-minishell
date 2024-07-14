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
Write to perror in the format "minishell: msg" and return boolian.
*/
bool	perror_and_bool(const char *msg, bool boolian)
{
	char	*perror_msg;

	if (msg)
		perror_msg = ft_strjoin("minishell: ", msg);
	else
		perror_msg = ft_strdup("minishell error");
	perror(perror_msg);
	free(perror_msg);
	return (boolian);
}

/*
Write to perror in the format "minishell: msg" and
return status.
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
"minishell: msg" to perror and exit with status.
(used for child processes)
*/
uint8_t	perror_and_exit_with_status(const char *msg, uint8_t status)
{
	char	*perror_msg;

	if (msg)
		perror_msg = ft_strjoin("minishell: ", msg);
	else
		perror_msg = ft_strdup("minishell error");
	perror(perror_msg);
	free(perror_msg);
	exit(status);
}
