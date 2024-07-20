/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pr_s.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:19:20 by lbrusa            #+#    #+#             */
/*   Updated: 2023/12/28 12:07:01 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
 hey |%2.s| |  |
 printf("str is %s\n",str);
 // printf("left padd = width %ld and prec %ld and conv %s pt = %d\n",width,
  prec, str, has_pt);
 */
ssize_t	get_left_padding(size_t width, size_t prec, char *str, int has_pt)
{
	ssize_t	i;
	char	*s;
	size_t	len;

	len = ft_strlen(str);
	if (width == 0 && !has_pt)
		width = len;
	if ((prec > len) && has_pt)
		prec = len;
	else if (has_pt)
		len = prec;
	if (len > width)
		width = (int)len;
	if (prec > width || (len > prec && has_pt))
		width = prec;
	s = safemalloc_and_set(width, ' ');
	if (!s)
		return (0);
	if (!has_pt)
		ft_memcpy((s + width - len), str, len);
	else
		ft_memcpy((s + width - prec), str, prec);
	i = write(1, s, width);
	free(s);
	return (i);
}

/*	
 width is: at least width or more chars - if the s is longer then
 the length of the string will be taken
 prec is: no more than prec char will be printed and the rest padded
 so if prec is zero then I will see only padding
 if both are zero nothing will be printed
 s = hello - if width is 8 and prec is zero then |        |
 s = hello - if width is 8 and prec is 1 then    |       h|
 s = hello - if width is 2 and prec is 8 then    |       h|
 // printf("right padd = width %ld and precision %ld and conv %s 
 pt = %d\n",width, prec, str, has_pt);
 */
ssize_t	get_right_padding(size_t width, size_t prec, char *str, int has_pt)
{
	ssize_t	i;
	char	*s;
	size_t	len;

	len = ft_strlen(str);
	if (prec > len)
		prec = len;
	if ((!has_pt) && (len > width))
		width = (int)len;
	if ((!has_pt) && (len > width))
		width = (int)len;
	else if ((has_pt) && (prec > width))
		width = prec;
	s = safemalloc_and_set(width, ' ');
	if (!s)
		return (0);
	ft_memcpy(s, str, len);
	if ((has_pt) && (width > prec))
		ft_memset(s + prec, ' ', width - prec);
	if ((has_pt) && (prec == 0))
		ft_memset(s, ' ', width);
	i = write(1, s, width);
	free(s);
	return (i);
}

/*
 Params:
 s : the string to print from the unnamed args
 wdh; the number of chars to print and will pad
 max_wdh; // n :the max number of chars to print and no pad
 '#' will give an error
 '-' will be ignored until the end or a dot
 I first check for the modifiers which are not allowed like #0h ljztL
 if after that I get numbers those are the width. (optional
 if a dot after that it means they specify the max width
 if after the dot there are no number the width will be zero!
 
 // 1 -  do the parameter field check
 // n$ - will skip for now if any number is followed by a $
 // 2 -check how many char if I have a dot followed by a number
 printf("va_arg is %s and conversion str: |%s| \n", str, conv);
 printf("\nconv str ==== |%s|\n",conv);
 printf("width %d and precision %d\n",width, prec);
 printf("width %d and precision %d\n",width, prec);
 */
ssize_t	pr_s(va_list *ap, char *conv)
{
	int		width;
	int		prec;
	char	*str;
	int		has_pt;

	width = 0;
	has_pt = 0;
	if (ft_strchr(conv, '*'))
		width = va_arg(*ap, int);
	if (ft_strchr(conv, '.'))
		has_pt = 1;
	get_width_and_precision(conv, &width, &prec);
	if (ft_strchr(conv, '-'))
		width *= -1;
	str = va_arg(*ap, char *);
	if ((str == NULL && !has_pt) || (str == NULL && prec >= 6 && has_pt))
		str = "(null)";
	if (width < 0)
		return (get_right_padding(-width, prec, str, has_pt));
	else
		return (get_left_padding(width, prec, str, has_pt));
}
