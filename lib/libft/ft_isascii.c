/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isascii.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:47:40 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/08 16:47:43 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
        #include <ctype.h>
		int isascii(int c);
* The isascii() function tests for an ASCII character, which is any 
* character between 0 and octal 0177 inclusive. (127!)
* The values returned are nonzero if the character c falls into the tested 
* class, and zero if not.
*/
int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
