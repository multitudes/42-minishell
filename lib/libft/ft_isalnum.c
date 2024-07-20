/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:35:59 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 17:59:25 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
       #include <ctype.h>
	   int isalnum(int c);
 The isalnum() function tests for any character for which isalpha(3) 
 or isdigit(3) is true.  The value of the
 argument must be representable as an unsigned 
 char or the value of EOF.
 EOF is a macro which expands to an integer constant expression with 
 type int and an implementation dependent negative value but is very 
 commonly -1
*/
int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
