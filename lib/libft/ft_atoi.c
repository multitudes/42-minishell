/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 13:45:15 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 17:56:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Signature:
       #include <stdlib.h>

       int atoi(const char *nptr);
	   
 The atoi() function converts the initial portion of the string pointed to 
 by str to int representation.
 It is equivalent to:
 (int)strtol(str, (char **)NULL, 10);
 The strtol() and strtol_l() functions are recommended instead of atoi() and 
 atoi_l() functions, especially in new code.
 The string may begin with an arbitrary amount of white space (as determined 
 by isspace(3)) followed by a single optional ‘+’ or ‘-’ sign.The remainder
 of the string is converted to a long, long long, intmax_t or quad_t
 value in the obvious manner, stopping at the first character which is not
 a valid digit in the given base.  (In bases above 10, the letter ‘A’
 in either upper or lower case represents 10, ‘B’ represents 11, and so forth, 
 with ‘Z’ representing 35.)
 The strtol(), strtoll(), strtoimax(), and strtoq() functions return 
 the result of the conversion, unless the value would underflow or overflow.
 If no conversion could be performed, 0 is returned
 isspace checks for      ``\t''``\n''``\v''``\f''``\r''`` ''
 */
int	ft_atoi(const char *str)
{
	int	n;
	int	sign;

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
