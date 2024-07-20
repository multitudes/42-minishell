/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:54:34 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:15:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
     #include <string.h>
     size_t strlcpy(char *dst, const char *src, size_t size);

 The strlcpy() and strlcat() functions copy and concatenate strings with
 the same input parameters and output result as snprintf(3).  They are
 designed to be safer, more consistent, and less error prone replacements
 for the easily misused functions strncpy(3) and strncat(3).
 strlcpy() and strlcat() take the full size of the destination buffer and
 guarantee NUL-termination if there is room.  Note that room for the NUL
 should be included in dstsize.  Also note that strlcpy() and strlcat()
 only operate on true ''C'' strings. This means that for strlcpy() src
 must be NUL-terminated and for strlcat() both src and dst() must be NUL-
 terminated.
 strlcpy() copies up to dstsize - 1 characters from the string src to dst,
 NUL-terminating the result if dstsize is not 0.
 The snprintf() and vsnprintf() functions will write at most size-1 of the
 characters printed into the output string (the size'th character then
 gets the terminating ‘\0’); if the return value is greater than or equal
 to the size argument, the string was too short and some of the printed
 characters were discarded.  The output is always null-terminated, unless
 size is 0.
 it returns the length of the src... yes !

 NOTE The orig strlcpy crashes when I pass NULL to either string args
 mine returns zero..
*/
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	srclen;

	if (dst == NULL || src == NULL)
		return (0);
	i = 0;
	srclen = ft_strlen(src);
	while ((i + 1) < dstsize && *src != '\0')
	{
		*dst++ = *src++;
		i++;
	}
	if (dstsize != 0)
		*dst = '\0';
	return (srclen);
}
