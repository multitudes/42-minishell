/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pr_c.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:19:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/17 20:00:03 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*

*/
ssize_t	get_left_padding_c(size_t width, int c)
{
	ssize_t	i;
	char	*s;

	s = safemalloc_and_set(width, ' ');
	if (!s)
		return (0);
	ft_memset(s, ' ', width);
	if (width > 0)
		ft_memcpy((s + width - 1), &c, 1);
	else
		ft_memcpy((s + width), &c, 1);
	if (!width)
		width = 1;
	i = write(1, s, width);
	free(s);
	return (i);
}

/*
 width is: at least width or more chars - if the s is longer then
 the length of the string will be taken
 max_w is: no more than max_w char will be printed and the rest padded
 so if max_w is zero then I will see only padding
 if both are zero nothing will be printed
 s = hello - if width is 8 and max_w is zero then |        |
 s = hello - if width is 8 and max_w is 1 then    |       h|
 s = hello - if width is 2 and max_w is 8 then    |       h|
 precision for character conversion (%c) is ignored.
 */
ssize_t	get_right_padding_c(size_t width, int c)
{
	ssize_t	i;
	char	*s;

	s = safemalloc_and_set(width, ' ');
	if (!s)
		return (0);
	ft_memset(s, ' ', width);
	ft_memcpy(s, &c, 1);
	i = write(1, s, width);
	free(s);
	return (i);
}

/*
 Params:
 c : the char to print from the unnamed args
 wdh; the number of chars to print and will pad
 max_wdh; // n :the max number of chars to print and no pad
 
 '#' will give an error
 '-' will be ignored until the end or a dot
 I first check for the modifiers which are not allowed like #0h ljztL
 if after that I get numbers those are the width. (optional
 if a dot after that it means they specify the max width
 if after the dot there are no number the width will be zero!
 printf("width %d\n",width);
 precision for character conversion (%c) is ignored.
   '#0+	' is ignored
 */
ssize_t	pr_c(va_list *ap, char *conv)
{
	int	width;
	int	c;

	width = 0;
	if (ft_strchr(conv, '*'))
		width = va_arg(*ap, int);
	else
		width = width_prec_atoi(&conv);
	if (ft_strchr(conv, '-'))
		width *= -1;
	c = va_arg(*ap, int);
	if (width < 0)
		return (get_right_padding_c(-width, c));
	else
		return (get_left_padding_c(width, c));
}
