/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 13:47:43 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/22 19:39:13 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));

 Parameters
 s:  The string on which to iterate.
 f:  The function to apply to each character.
 Return value
 The string created from the successive applications
 of ’f’.
 Returns NULL if the allocation fails.
 Applies the function ’f’ to each character of the
 string ’s’, and passing its index as first argument
 to create a new string (with malloc(3)) resulting
 from successive applications of ’f’.
 	// if (s == NULL || f == NULL)
	// 	return ((char *)s);
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	count;
	char	*s2;
	size_t	i;

	i = 0;
	count = ft_strlen(s);
	s2 = malloc((count + 1) * sizeof(char));
	if (s2 == NULL)
		return (NULL);
	while (count > i)
	{
		s2[i] = f(i, s[i]);
		i++;
	}
	s2[i] = 0;
	return (s2);
}
