/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:49:41 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/10 12:51:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
/*
SYNOPSIS
       #include <string.h>
       int strncmp(const char *s1, const char *s2, size_t n);

 The strcmp() and strncmp() functions lexicographically compare the 
 null-terminated strings s1 and s2.
 The strncmp() function compares not more than n characters.  
 Because strncmp() is designed for comparing strings rather than binary data,
 characters that appear after a ‘\0’ character are not compared.
 RETURN VALUES
 The strcmp() and strncmp() functions return an integer greater than, 
 equal to, or less than 0, according as the string s1 is greater than,
 equal to, or less than the string s2.
 The comparison is done using unsigned characters, so that ‘\200’
 is greater than ‘\0’.
 I check the input args for null values but the orig function has 
 this check already made through the preprocessor compiler so i cannot
 pass NULL even if I wanted to
 */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (s1 == NULL || s2 == NULL)
		return (0);
	i = 0;
	if (n == 0)
		return (0);
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

/*
works like strncmp but with less code
*/
int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 && !s2)
		return (0);
	if (!s1 && s2)
		return (-(unsigned char)s2[0]);
	if (s1 && !s2)
		return ((unsigned char)s1[0]);
	while (s1[i] == s2[i] && s1[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
