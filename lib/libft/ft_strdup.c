/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:57:14 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:14:45 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
     #include <string.h>
    char *strdup(const char *s);

The strdup() function allocates sufficient memory for a copy of the string s1, 
does the copy, and returns a pointer to it.  The pointer may subsequently 
be used as an argument to the function free(3).
If insufficient memory is available, NULL is returned and errno is set to ENOMEM.
The strndup() function copies at most n characters from the string s1 always NUL 
terminating the copied string.
 */
char	*ft_strdup(const char *s1)
{
	size_t	i;
	size_t	len;
	char	*s2;

	if (s1 == NULL)
		return (NULL);
	i = 0;
	len = ft_strlen(s1);
	s2 = malloc(len + 1);
	if (s2 == NULL)
		return (NULL);
	while (i < len)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
