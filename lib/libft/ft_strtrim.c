/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:57:47 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:13:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
char *ft_strtrim(char const *s1, char const *set);

 s1:  The string to be trimmed.
 set:  The reference set of characters to trim.
 Return value
 The trimmed string.
 NULL if the allocation fails.
 Description
 Allocates (with malloc(3)) and returns a copy of
 ’s1’ with the characters specified in ’set’ removed
 from the beginning and the end of the string.
 */
char	*ft_strtrim(char const *s1, char const *set)
{
	int		n;
	int		b;
	char	*s2;

	if (s1 == NULL || set == NULL)
		return (NULL);
	b = 0;
	n = ft_strlen(s1) - 1;
	while (n > 0 && (ft_strchr(set, s1[n]) != NULL))
		n--;
	while (b <= n && (ft_strchr(set, s1[b]) != NULL))
		b++;
	s2 = ft_substr(s1, b, n - b + 1);
	return (s2);
}

// #include <stdio.h>
// int main()
// {

// 	char *s = ft_strtrim("   xxxa   xxx", " x");
// 	printf("|   xxx   xxx| with x is -%s-\n",s);
// 		free(s);

// 	s = ft_strtrim("   xxxldhug", " x");
// 	printf("str is -%s-",s);
// 		free(s);
// 	s = ft_strtrim("a", " x");
// 	printf("str is -%s-",s);
// 	free(s);
// 	return (0);
// }
