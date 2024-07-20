/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pr_.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:18:55 by lbrusa            #+#    #+#             */
/*   Updated: 2023/12/27 16:04:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
 function responsible for the %% case
 
 It will print % if successul
 the conv string in input can be %% or |%--+02%|
 */
ssize_t	pr_(va_list *ap, char *conv)
{
	(void)ap;
	(void)conv;
	return (write(1, "%", 1));
}
