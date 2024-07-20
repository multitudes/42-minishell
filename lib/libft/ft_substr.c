/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:16:37 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:09:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
char *ft_substr(char const *s, unsigned int start, size_t len);

 s:  The string from which to create the substring.
 start:  The start index of the substring in the
 string ’s’.
 len:  The maximum length of the substring.
 Allocates (with malloc(3)) and returns a substring
 from the string ’s’.
 The substring begins at index ’start’ and is of
 maximum size ’len’.
 ex hello len 5
 start is 4 -> o
 possible len is 1 5-4
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	strlen;
	char	*sub;

	if (s == NULL)
		return (NULL);
	i = 0;
	strlen = ft_strlen(s);
	if (len + (size_t)start > strlen)
	{
		if (strlen >= (size_t)start)
			len = strlen - (size_t)start;
		else
			len = 0;
	}
	sub = ft_calloc(len + 1, 1);
	if (sub == NULL)
		return (NULL);
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	return (sub);
}
