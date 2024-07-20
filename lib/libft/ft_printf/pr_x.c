/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pr_x.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:19:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/17 19:56:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*

*/
ssize_t	pr_x(va_list *ap, char *conv)
{
	int		nr;
	t_flags	*flags;

	flags = init_flags();
	if (flags == NULL)
		return (0);
	fill_flags(conv, &flags);
	if (ft_strchr(conv, '#') != NULL)
		flags->hash = 1;
	if (flags->star)
		flags->fw = va_arg(*ap, int);
	nr = va_arg(*ap, unsigned int);
	if (flags->minus)
		return (get_r_pad_i(ft_itox(nr), flags));
	else
		return (get_l_pad_i(ft_itox(nr), flags));
}
