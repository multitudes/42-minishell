/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:39:29 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:14:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS

char *ft_strjoin(char const *s1, char const *s2)

 s1:  The prefix string.
 s2:  The suffix string.
 ret The new string.
 NULL if the allocation fails.
 Allocates (with malloc(3)) and returns a new
 string, which is the result of the concatenation
 of ’s1’ and ’s2’.
 */
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*cat;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	cat = ft_calloc(len1 + len2 + 1, 1);
	if (cat == NULL)
		return (NULL);
	ft_strlcat(cat, s1, len1 + 1);
	ft_strlcat(cat, s2, len1 + len2 + 1);
	return (cat);
}
