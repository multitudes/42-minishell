/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 14:26:42 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:18:05 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS

void ft_striteri(char *s, void (*f)(unsigned int, char*));

 Parameters
 s:  The string on which to iterate.
 f:  The function to apply to each character.
 Description
 Applies the function ’f’ on each character of
 the string passed as argument, passing its index
 as first argument.  Each character is passed by
 address to ’f’ to be modified if necessary.
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t	count;

	if (s == NULL || f == NULL)
		return ;
	count = ft_strlen(s);
	while (count > 0)
	{
		count--;
		f(count, &s[count]);
	}
}
