/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:21:44 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/17 19:12:05 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
ops is an array of function pointers each taking care of a 
conversion string.
the conversion specifiers for the mandatory part are:
|%cspdiuxX|
if none are found in the format specifier string then the last one
will be executed err_s
*/
void	init_ops(t_ffunc *ops)
{
	ops[0] = pr_;
	ops[1] = pr_c;
	ops[2] = pr_s;
	ops[3] = pr_p;
	ops[4] = pr_i;
	ops[5] = pr_i;
	ops[6] = pr_u;
	ops[7] = pr_x;
	ops[8] = pr_xx;
	ops[9] = err_s;
}

/*
t_flags is a struct used in the %i conversion
when everything became too unwieldly 
*/
t_flags	*init_flags(void)
{
	t_flags	*flags;

	flags = malloc(sizeof(t_flags));
	if (flags == NULL)
		return (NULL);
	flags->minus = 0;
	flags->zero = 0;
	flags->plus = 0;
	flags->space = 0;
	flags->dot = 0;
	flags->minw = 0;
	flags->padw = 0;
	flags->fw = 0;
	flags->len = 0;
	flags->prec = 0;
	flags->star = 0;
	flags->hash = 0;
	flags->hashhash = 0;
	flags->start = 0;
	flags->err = 0;
	flags->pad = ' ';
	flags->unsgned = 0;
	return (flags);
}

/*
 this is a variation of ft_atoi and it is made in such a way
 that if I pass a string, any initial chars which are not a number 
 and including beginning zeroes will be discarded. 
 It is only used for width and precision in strings 
 the precision will start usually with a '.' and some numbers and this 
 will work there. if there are no numbers or if I 
 passed a null terminator then it will return zero
 */
int	width_prec_atoi(char **p)
{
	int	n;
	int	sign;

	n = 0;
	sign = 1;
	if (**p == 0 || *p == 0 || p == 0)
		return (0);
	while ((!ft_isdigit(**p) || **p == '0') && **p != 0)
	{
		if (**p == '-')
			sign = -1;
		(*p)++;
	}
	while (ft_isdigit(**p))
		n = 10 * n + ((*(*p)++ - '0') * sign);
	return (n);
}

/*
 running out of space in my func I had to do this extra func
 to save two lines
 If my malloc fails to allocated width chars then it frees p
 and returns null. Otherwise it returns a null terminated blank string
 */
char	*safemalloc_and_set(ssize_t width, int c)
{
	char	*s;

	s = malloc(sizeof(char) * (width + 1));
	if (!s)
		return (NULL);
	s = ft_memset((char *)s, c, width);
	s[width] = 0;
	return (s);
}

/*
used in the string conversion without the flags struct
 */
void	get_width_and_precision(char *conv, int *width, int *prec)
{
	if (*conv == '-')
		conv++;
	if (*conv == '.' && conv++)
	{
		*width = 0;
		*prec = width_prec_atoi(&conv);
	}
	else
	{
		*width = width_prec_atoi(&conv);
		if (*(conv) == '.')
			*prec = width_prec_atoi(&conv);
		else
			*prec = *width;
	}
}
