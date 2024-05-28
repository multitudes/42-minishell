/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:22:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 11:49:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

/*
Used to returnfrom the program with an error message. and exit 1
if data is not null will update the error exit code
*/
int	error_set_status(char *msg, t_data *data)
{
	write(2, msg, ft_strlen(msg));
	if (data)
		data->exit_status = 1;
	return (EXIT_FAILURE);
}

/*
will exit the program with an error message
*/
int	exit_err_failure(char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(EXIT_FAILURE);
}
