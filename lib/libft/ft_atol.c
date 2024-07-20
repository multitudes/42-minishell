/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 13:45:15 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/06 15:19:14 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Signature:
    #include <stdlib.h>

    long
    atol(const char *str);
	   
The atol() function converts the initial portion of the string pointed to
by str to long integer representation.

 The string may begin with an arbitrary amount of white space (as determined 
 by isspace(3)) followed by a single optional ‘+’ or ‘-’ sign.The remainder
 of the string is converted to a long, long long, intmax_t or quad_t
 value in the obvious manner, stopping at the first character which is not
 a valid digit.
 isspace checks for      ``\t''``\n''``\v''``\f''``\r''`` ''
 */
long	ft_atol(const char *str)
{
	long	n;
	int		sign;

	if (str == NULL)
		return (0);
	n = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || (*str == 32))
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign *= -1;
	while (ft_isdigit(*str))
		n = 10 * n + ((*str++ - '0') * sign);
	return (n);
}

// int main(void)
// {
// 	long num = 0;
// 	num = ft_atol("2147483650");
// 	printf("num got %ld",num);
// 	return 0;
// }