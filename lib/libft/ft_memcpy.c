/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 12:05:12 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/22 18:18:51 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
       #include <string.h>

       void *memcpy(void *dest, const void *src, size_t n);

 The memcpy() function copies n bytes from memory area src to memory area
 dst.  If dst and src overlap, behavior is undefined.  Applications in
 which dst and src might overlap should use memmove(3) instead.
 
 RETURN VALUES
 The memcpy() function returns the original value of dst.
*/
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*tmp_dst;
	unsigned char	*tmp_src;

	if (dst == NULL || src == NULL)
		return (dst);
	tmp_dst = (unsigned char *) dst;
	tmp_src = (unsigned char *) src;
	while (n > 0)
	{
		*tmp_dst++ = *tmp_src++;
		n--;
	}
	return (dst);
}
