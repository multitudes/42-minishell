/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:42:50 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:17:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
       #include <string.h>

     char *strnstr(const char *big, const char *little, size_t len);

The strnstr() function locates the first occurrence of the null-terminated 
 string needle in the string haystack, where not more than len characters
 are searched.  Characters that appear after a ‘\0’ character
 are not searched.  Since the strnstr() function is a FreeBSD specific API, 
 it should only be used when portability is not a concern.
 If needle is an empty string, haystack is returned; if needle occurs nowhere 
 in haystack, NULL is returned;
 otherwise a pointer to the first character of the first occurrence of needle 
 is returned.
 */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	k;

	if (haystack == NULL || needle == NULL)
		return (NULL);
	i = 0;
	j = 0;
	k = 0;
	if (ft_strlen(needle) == 0)
		return ((char *)haystack);
	while (i < len && haystack[i] != '\0')
	{
		j = i;
		k = 0;
		while (haystack[j] == needle[k] && needle[k] != '\0' && j < len)
		{
			j++;
			k++;
		}
		if (k > 0 && needle[k] == '\0')
			return ((char *)haystack + i);
		i++;
	}
	return (NULL);
}
