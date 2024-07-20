/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoxp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:48:35 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/06 13:54:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Used for converting pointers to base16
 */
char	*ft_itoxp(uintptr_t nbr)
{
	int		i;
	char	*prefix;
	char	hex_p[19];
	char	*res;

	i = 0;
	prefix = "0x";
	if (nbr == 0)
		return (ft_strdup("(nil)"));
	while (nbr > 0)
	{
		hex_p[i++] = BASE16[nbr % 16];
		nbr /= 16;
	}
	_reverse(hex_p, i);
	res = ft_strjoin(prefix, hex_p);
	return (res);
}
