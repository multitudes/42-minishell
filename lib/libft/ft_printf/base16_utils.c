/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base16_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:21:44 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/31 11:39:26 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
Used for conversion to base16
if the hash is used then I add a prefix.
This function handles the lowercase x
 */
// char	*ft_itox(unsigned int nbr)
// {
// 	int		i;
// 	char	hex_p[19];
// 	char	*res;

// 	i = 0;
// 	if (nbr == 0)
// 		hex_p[i++] = '0';
// 	while (nbr > 0)
// 	{
// 		hex_p[i++] = BASE16[nbr % 16];
// 		nbr /= 16;
// 	}
// 	reverse(hex_p, i);
// 	res = ft_strdup(hex_p);
// 	return (res);
// }

/*
Used for conversions to base16
if the hash is used then I add a prefix.
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
	reverse(hex_p, i);
	res = ft_strdup(hex_p);
	return (res);
}

/*
Used for converting unsigned int in the u conversion
I do not take into account the minus but the max amount of chars is the same
 */
char	*ft_itou(unsigned int nbr)
{
	int		i;
	char	res[13];

	i = 0;
	if (nbr == 0)
		res[i++] = '0';
	while (nbr > 0)
	{
		res[i++] = '0' + (nbr % 10);
		nbr /= 10;
	}
	reverse(res, i);
	return (ft_strdup(res));
}

/*

*/
void	update_for_x(char *s, char *conv, t_flags *flags)
{
	if (flags->hash)
	{
		ft_memcpy(s + 2, conv, ft_strlen(conv));
		ft_memcpy(s, "0x", 2);
	}
}
