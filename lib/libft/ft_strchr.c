/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 18:44:53 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/22 18:33:14 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
      #include <string.h>
      char *strchr(const char *s, int c);
 
 DESCRIPTION
 The strchr() function locates the first occurrence of 
 c (converted to a char) in the string pointed to by
 s.  The terminating null character is considered 
 to be part of the string; therefore if c is ‘\0’, the
 functions locate the terminating ‘\0’.
 The strrchr() function is identical to strchr(), 
 except it locates the last occurrence of c.
 RETURN VALUES
 The functions strchr() and strrchr() return a pointer to the located 
 character, or NULL if the character does not appear in the string.
 */
char	*ft_strchr(const char *s, int c)
{
	if (s == NULL)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}
