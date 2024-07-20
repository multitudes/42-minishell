/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 17:53:52 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:02:05 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
char *ft_itoa(int n);

 n:  the integer to convert.
 Return value
 The string representing the integer.
 NULL if the allocation fails.
 Description
 Allocates (with malloc(3)) and returns a string
 representing the integer received as an argument.
 Negative numbers must be handled.
 
 here I decide something different. since the neg int min is the one giving
 problems I assumed the default is a neg number, and I change the sign only
 if it is a positive number. So I run all the calculations like n is negative
 and if it wasnt at the end i dont print the minus and overflow not a prob
 anymore!
 */
char	*ft_itoa(int n)
{
	int		i;
	int		minus;
	char	temp[12];

	i = 1;
	minus = 1;
	temp[11] = '\0';
	if (n >= 0)
	{
		n *= -1;
		minus = 0;
	}
	temp[11 - i++] = '0' - (n % 10);
	n /= 10;
	while (n != 0)
	{
		temp[11 - i++] = '0' - (n % 10);
		n /= 10;
	}
	if (minus)
		temp[11 - i++] = '-';
	return (ft_strdup(&temp[11 - i + 1]));
}
