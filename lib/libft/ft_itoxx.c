/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoxx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:44:35 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/06 13:54:32 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Used for conversions to base16
This function handles the uppercase X
 */
char	*ft_itoxx(unsigned int nbr)
{
	int		i;
	char	hex_p[19];
	char	*res;

	i = 0;
	if (nbr == 0)
		hex_p[i++] = '0';
	while (nbr > 0)
	{
		hex_p[i++] = BASE16X[nbr % 16];
		nbr /= 16;
	}
	_reverse(hex_p, i);
	res = ft_strdup(hex_p);
	return (res);
}
