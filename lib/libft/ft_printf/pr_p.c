/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pr_p.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:19:14 by lbrusa            #+#    #+#             */
/*   Updated: 2023/12/28 12:07:01 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*

 */
ssize_t	get_l_pad_p(int width, char *p)
{
	ssize_t	i;
	char	*s;
	size_t	len;
	char	pad;

	pad = ' ';
	len = ft_strlen(p);
	if (((int)len) > width)
		width = ((int)len);
	s = safemalloc_and_set(width, pad);
	if (!s)
		return (0);
	ft_memcpy((s + width - (len)), p, len);
	free(p);
	i = write(1, s, width);
	free(s);
	return (i);
}

/*

 */
ssize_t	get_r_pad_p(int width, char *p)
{
	ssize_t	i;
	char	*s;
	size_t	len;
	char	pad;

	pad = ' ';
	len = ft_strlen(p);
	if (((int)len) > width)
		width = ((int)len);
	s = safemalloc_and_set(width, pad);
	if (!s)
		return (0);
	ft_memcpy(s, p, len);
	free(p);
	i = write(1, s, width);
	free(s);
	return (i);
}

/*

 */
ssize_t	pr_p(va_list *ap, char *conv)
{
	uintptr_t	nr_input;
	int			width;

	if (ft_strchr(conv, '*'))
		width = va_arg(*ap, int);
	else
		width = width_prec_atoi(&conv);
	if (ft_strchr(conv, '-'))
		width *= -1;
	if (!width)
		width = 1;
	nr_input = va_arg(*ap, uintptr_t);
	if (width < 0)
		return (get_r_pad_p(-width, ft_itoxp(nr_input)));
	else
		return (get_l_pad_p(width, ft_itoxp(nr_input)));
}
