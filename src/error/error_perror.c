/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_perror.c                                      :+:      :+:    :+:   */
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
Writes "minishell: msg" to perror.
If msg argument is NULL, "minishell error" is passed to perror.
*/
// void	perror_minishell(const char *msg)
// {
// 	char	*perror_msg;

// 	if (msg)
// 		perror_msg = ft_strjoin("minishell: ", msg);
// 	else
// 		perror_msg = ft_strdup("minishell error");
// 	perror(perror_msg);
// 	free(perror_msg);
// }
/*
Writes "minishell: msg" to perror and returns boolean.
If msg argument is NULL, "minishell error" is passed to perror.
*/
bool	perror_and_bool(const char *msg, bool boolean)
{
	char	*perror_msg;

	if (msg)
		perror_msg = ft_strjoin("minishell: ", msg);
	else
		perror_msg = ft_strdup("minishell error");
	perror(perror_msg);
	free(perror_msg);
	return (boolean);
}

/*
Writes "minishell: msg" to perror and returns status.
If msg argument is NULL, "minishell error" is passed to perror.
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
Writes "minishell: msg_1: msg_2" to perror and returns status.
If msg arguments are NULL, "minishell error" is passed to perror.
*/
uint8_t	perror_and_status2(const char *msg_1, const char *msg_2, \
							uint8_t status)
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
Writes "minishell: msg" to perror and exits with status.
(used for child processes)
If msg argument is NULL, "minishell error" is passed to perror.
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

/*
Writes "minishell: msg" to perror and returns NULL.
If msg argument is NULL, "minishell error" is passed to perror.
*/
void	*perror_and_null(const char *msg)
{
	char	*perror_msg;

	if (msg)
		perror_msg = ft_strjoin("minishell: ", msg);
	else
		perror_msg = ft_strdup("minishell error");
	perror(perror_msg);
	free(perror_msg);
	return (NULL);
}
