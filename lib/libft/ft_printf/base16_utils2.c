/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base16_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:21:44 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/31 11:39:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
In the functions used to convert a number to 
a string I use an algo which produces a string first 
but in the reverse order. This function just reverses that string 
*/
void	reverse(char *res, int len)
{
	int	j;
	int	c;
	int	i;

	i = len;
	j = 0;
	i--;
	while (j < i)
	{
		c = res[j];
		res[j++] = res[i];
		res[i--] = c;
	}
	res[len] = 0;
}

/*
Used for converting pointers to base16
 */
// char	*ft_itoxp(uintptr_t nbr)
// {
// 	int		i;
// 	char	*prefix;
// 	char	hex_p[19];
// 	char	*res;

// 	i = 0;
// 	prefix = "0x";
// 	if (nbr == 0)
// 		return (ft_strdup("(nil)"));
// 	while (nbr > 0)
// 	{
// 		hex_p[i++] = BASE16[nbr % 16];
// 		nbr /= 16;
// 	}
// 	reverse(hex_p, i);
// 	res = ft_strjoin(prefix, hex_p);
// 	return (res);
// }
