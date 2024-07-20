/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:54:23 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:01:07 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
       #include <ctype.h>
		int isprint(int c);
 The isprint() function tests for any printing character, 
 including space (‘ ’).  The value of the argument must be representable 
 as an unsigned char or the value of EOF.
 
 In the ASCII character set, this includes the following characters 
 (preceded by their numeric values, in octal):
 
 040 sp to 176 ``~'' => 32 to 126!
 
 RETURN VALUES
 The isprint() function returns zero if the character tests 
 false and returns non-zero if the character tests true.
 */
int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}
