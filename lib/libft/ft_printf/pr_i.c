/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pr_i.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:19:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/17 19:59:49 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	prepare_flags2(char *conv, t_flags *flags)
{
	if ((flags->plus || flags->space) && !flags->unsgned)
		if (flags->len + 1 > flags->fw)
			flags->fw = flags->len + 1;
	if ((flags->pad == '0') && (flags->len > flags->prec))
		flags->pad = ' ';
	if ((flags->hash || flags->hashhash) && !(conv[0] == '0') && \
	((flags->fw < (flags->len + 2)) || (flags->fw < (flags->prec + 2))))
	{
		if (flags->fw < (flags->len + 2))
			flags->fw = flags->len + 2;
		if (flags->fw < (flags->prec + 2))
			flags->fw = flags->prec + 2;
	}
}

void	prepare_flags(char *conv, t_flags *flags)
{
	flags->len = (int)ft_strlen(conv);
	if (flags->prec < flags->len)
		if (conv[0] == '-')
			flags->prec = flags->len - 1;
	if (conv[0] == '-')
	{
		flags->plus = 0;
		flags->space = 0;
	}
	if ((flags->fw == (flags->prec)) && (conv[0] == '-') && (flags->dot))
		(flags->fw)++;
	if (flags->len > (flags->fw))
		flags->fw = flags->len;
	prepare_flags2(conv, flags);
}

/*

 */
ssize_t	get_l_pad_i(char *conv, t_flags *flags)
{
	ssize_t	i;
	char	*s;

	prepare_flags(conv, flags);
	s = safemalloc_and_set(flags->fw, ' ');
	if (!s)
		return (0);
	ft_memset(s + flags->fw - flags->prec, flags->pad, flags->prec);
	adjust_l_i(s, conv, flags);
	free(conv);
	i = write(1, s, flags->fw);
	free(s);
	free(flags);
	return (i);
}

/*

 */
ssize_t	get_r_pad_i(char *conv, t_flags *flags)
{
	ssize_t	i;
	char	*s;

	update_flags_r_i(conv, flags);
	s = safemalloc_and_set(flags->fw, flags->pad);
	if (!s)
		return (0);
	adjust_r_i(s, conv, flags);
	i = write(1, s, flags->fw);
	free(s);
	free(flags);
	free(conv);
	return (i);
}

/*
 In C's printf and related functions, the length modifiers \
 specify the size of the argument that you're working
 with. Here's a list of common length modifiers:
 
 h: Used with integer specifiers (d, i, o, u, x, X) to indicate 
 that the corresponding argument is a short (for d, i, o, u, x, X)
 or unsigned short (for o, u, x, X).
 hh: Used with integer specifiers to indicate that the corresponding 
 argument is a signed char (for d, i) or unsigned char (for o, u, x, X).
 l: Used with integer specifiers to indicate that the corresponding 
 argument is a long (for d, i, o, u, x, X), or with floating-point
 specifiers (f, e, E, g, G) to indicate that the corresponding argument
 is a double.
 ll: Used with integer specifiers to indicate that the corresponding 
 argument is a long long (for d, i, o, u, x, X).
 j: Used with integer specifiers to indicate that the corresponding 
 argument is of type intmax_t or uintmax_t.
 z: Used with integer specifiers to indicate that the corresponding 
 argument is of type size_t.
 t: Used with integer specifiers to indicate that the corresponding 
 argument is of type ptrdiff_t.
 if (zero)
 printf("got zero\n");
 printf("width %d\n",width);
 if (ft_strchr(conv, '-') && ft_strchr(conv, ' ') == NULL)
 width *= -1;
 printf("im here in i! and the value is %s\n", nr);
 printf("width %d zero %d sign %d\n",width, zero, pl_sign);
 */
ssize_t	pr_i(va_list *ap, char *conv)
{
	int		nr_input;
	t_flags	*flags;

	flags = init_flags();
	if (flags == NULL)
		return (0);
	fill_flags(conv, &flags);
	if (flags->star)
		flags->fw = va_arg(*ap, int);
	nr_input = va_arg(*ap, int);
	if (flags->minus)
		return (get_r_pad_i(ft_itoa((int)nr_input), flags));
	else
		return (get_l_pad_i(ft_itoa((int)nr_input), flags));
}
