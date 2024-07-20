/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 12:52:03 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:18:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
       #include <string.h>

       void *memmove(void *dest, const void *src, size_t n);

 DESCRIPTION
 The memmove() function copies len bytes from string src to string dst.
 The two strings may overlap; the copy is always done in a non-destructive
 manner.
 RETURN VALUES
 The memmove() function returns the original value of dst.
 there is a way to make it faster without intermediate copy
 checking if the strings could overlapt or if they need to be copied
 from the end or the front
 I use len which is unsigned to loop on the strings
 */
void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*tmp_dst;
	unsigned char	*tmp_src;

	if (dst == NULL || src == NULL || len == 0)
		return (dst);
	tmp_dst = (unsigned char *) dst;
	tmp_src = (unsigned char *) src;
	if (tmp_dst > tmp_src)
	{
		while (len > 0)
		{
			*(tmp_dst + len - 1) = *(tmp_src + len - 1);
			len--;
		}
	}
	else if (tmp_src > tmp_dst)
	{
		while (len > 0)
		{
			*tmp_dst++ = *tmp_src++;
			len--;
		}
	}
	return (dst);
}
