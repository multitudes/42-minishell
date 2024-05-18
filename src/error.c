/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:22:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/18 12:08:36 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

/*
Used to exit the program with an error message. and exit 1
*/
int	_exit_err(char *msg)
{
	write(1, msg, ft_strlen(msg));
	return (1);
}