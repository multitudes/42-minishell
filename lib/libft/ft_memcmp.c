/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:02:02 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:19:24 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
       #include <string.h>

       int memcmp(const void *s1, const void *s2, size_t n);
	   
DESCRIPTION
The memcmp() function compares byte string s1 against byte string s2.
Both strings are assumed to be n bytes long.
RETURN VALUES
 The memcmp() function returns zero if the two strings are identical, 
 otherwise returns the difference
 between the first two differing bytes (treated as unsigned char values, 
 so that ‘\200’ is greater than ‘\0’, for example).
 Zero-length strings are always identical.
 This behavior is not required by C and portable code should only depend on the
 sign of the returned value.
 */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	if ((s1 != NULL && s2 != NULL) && (n > 0))
	{
		while ((i < (n - 1)) && *(unsigned char *)s1 == *(unsigned char *)s2)
		{
			s1++;
			s2++;
			i++;
		}
		return (*(unsigned char *)s1 - *(unsigned char *)s2);
	}
	return (0);
}
