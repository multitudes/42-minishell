/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:19:14 by lbrusa            #+#    #+#             */
/*   Updated: 2023/12/29 12:07:41 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
 Params:
 will print the portion of the string giving a problem.
 This is the same behaviour of the orig printf.
 for instance %r will print just %r
 or if I have a memory error in creating the conv string
 I will have conv == NULL and a mem error
 */
ssize_t	err_s(va_list *ap, char *conv)
{
	(void)ap;
	write(1, "%", 1);
	if (conv)
	{
		write(1, conv, ft_strlen(conv));
		free(conv);
	}
	return (1);
}
