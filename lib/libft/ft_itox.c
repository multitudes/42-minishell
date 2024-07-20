/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:44:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/06 13:54:25 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
In the functions used to convert a number to 
a string I use an algo which produces a string first 
but in the reverse order. This function just reverses that string 
*/
void	_reverse(char *res, int len)
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
Used for conversion to base16 from unsigned int
This function handles the lowercase x
 */
char	*ft_itox(unsigned int nbr)
{
	int		i;
	char	hex_p[19];
	char	*res;

	i = 0;
	if (nbr == 0)
		hex_p[i++] = '0';
	while (nbr > 0)
	{
		hex_p[i++] = BASE16[nbr % 16];
		nbr /= 16;
	}
	_reverse(hex_p, i);
	res = ft_strdup(hex_p);
	return (res);
}
