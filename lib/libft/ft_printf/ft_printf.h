/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:18:41 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/31 11:38:41 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft.h"
# include <stdarg.h>
# include <stdlib.h>
# include <stdio.h>

// I decided to group all flags in one struct to avoid getting crazy
//
typedef struct s_flags {
	int		minus;
	int		zero;
	int		plus;
	int		space;
	int		dot;
	int		minw;
	int		padw;
	int		fw;
	int		len;
	int		prec;
	int		star;
	int		hash;
	int		hashhash;
	int		start;
	int		err;
	char	pad;
	int		unsgned;
}	t_flags;

/*
 Bonus list:
 
 • Manage any combination of the following flags: ’-0.’
 and the field minimum width under all conversions.
 • Manage all the following flags: ’# +’ (Yes, one of them is a space)
 
 An integer between % and the format code sets a minimum field width.
 The padding ensures that it reaches a certain minimum length.
 If the value is longer than that minimum, it will still be printed in full.
 The default padding is done with spaces.
 If you wish to pad with 0's, place a 0 before the field width specifier.
 For example, %05d will pad a number of less than five digits with 0's so that 
 its total length is five.
 */

// these are the mandatory conversions for the proj.
# define CONVS "%cspdiuxX"

// Used by the functions converting to hex with capital letters and not
# define BASE16 "0123456789abcdef"
# define BASE16X "0123456789ABCDEF"

// when I update the CONVS array I need to update these in the ops
// initialisation function and the conv Error is the last in the array
// this is why max ops is one higher
# define CONV_ERROR 9
# define MAX_OPS 10

/*
historically important but not used in the program now because printf 
gives out warnings not errors
*/
# define MEM_ERR "Memory error\n"
# define ERROR_1 "flag '#' results in undefined behavior with 's'\
 conversion specifier"
# define ERROR_2 "Some modifiers results in undefined behavior or no\
 effect with 's' conversion specifier "
# define INV_CONV "ft_printf: invalid directive "
# define ERR_C_PRECI "Precision used with 'c' conversion specifier, \
resulting in undefined behavior\n"
# define ERR_C_HASH "Flag '#' results in undefined behavior with 'c' \
conversion specifier\n"
# define ERR_C_PLUS "Flag '+' results in undefined behavior with 'c' \
conversion specifier\n"
# define ERR_C_ZERO "Flag '0' results in undefined behavior with 'c' \
conversion specifier\n"
# define ERR_S_ZERO "Flag '0' results in undefined behavior with 's' \
conversion specifier\n"
# define ERR_SPACE "Flag ' ' results in undefined behavior with \
conversion specifier\n"
# define ERR_DOUBLE_STAR "Invalid conversion specifier '*'\n"
# define GEN_ERR_CONV "Invalid conversion specifier\n"
# define ERR_S_PLUS "Flag '+' results in undefined behavior with 's' \
conversion specifier\n"
# define ERR_I_HASH "Flag '#' results in undefined behavior with 'i' \
conversion specifier\n"
# define ERR_P_HASH "Flag '#' results in undefined behavior with 'p' \
conversion specifier\n"
# define ERR_U_HASH "Flag '#' results in undefined behavior with 'p' \
conversion specifier\n"
# define ERR_P_ZERO "Flag '0' results in undefined behavior with 'p' \
conversion specifier\n"
# define ERR_P_PLUS "Flag '+' results in undefined behavior with 'p' \
conversion specifier\n"
# define ERR_U_PLUS "Flag '+' results in undefined behavior with 'u' \
conversion specifier\n"
# define ERR_P_SPACE "Flag ' ' results in undefined behavior with 'p' \
conversion specifier\n"
# define ERR_U_SPACE "Flag ' ' results in undefined behavior with 'u' \
conversion specifier\n"
# define ERR_MINUS_ZERO "flag '0' is ignored when flag '-' is present\n"
# define ERR_SPACE_PLUS "flag ' ' is ignored when flag '+' is present\n"
# define ERR_INT_OVER "Format specifies type 'int' but the \
argument has type 'long'\n"
# define ERR_I_ZERO_MINUS "flag '0' is ignored when flag '-' is present\n"

// ffunc is for formatter function
// functions with these signatures will be passed in an array
typedef ssize_t	(*t_ffunc)(va_list *, char *);
ssize_t		pr_(va_list *ap, char *conv);
ssize_t		pr_c(va_list *ap, char *conv);
ssize_t		pr_s(va_list *ap, char *conv);
ssize_t		pr_p(va_list *ap, char *conv);
ssize_t		pr_d(va_list *ap, char *conv);
ssize_t		pr_u(va_list *ap, char *conv);
ssize_t		pr_i(va_list *ap, char *conv);
ssize_t		pr_u(va_list *ap, char *conv);
ssize_t		pr_x(va_list *ap, char *conv);
ssize_t		pr_xx(va_list *ap, char *conv);
ssize_t		err_s(va_list *ap, char *conv);

int			ft_printf(const char *format, ...);
void		init_ops(t_ffunc *ops);
int			getops_nr(char *p, char **conv, int *ii);
ssize_t		get_conv(char *format, int *ii, va_list *ap);
ssize_t		get_l_pad_p(int width, char *p);
ssize_t		get_r_pad_p(int width, char *p);
ssize_t		get_l_pad_i(char *p, t_flags *flags);
ssize_t		get_r_pad_i(char *p, t_flags *flags);
ssize_t		get_left_padding_c(size_t width, int c);
ssize_t		get_right_padding_c(size_t width, int c);
ssize_t		get_right_padding(size_t width, size_t max_w, char *p, int has_pt);
ssize_t		get_left_padding(size_t width, size_t max_w, char *p, int has_pt);
void		adjust_l_i(char *s, char *p, t_flags *flags);
void		adjust_l_i2(char *s, char *p, t_flags *flags);
void		adjust_l_i3(char *s, char *p, t_flags *flags);
void		adjust_r_i(char *s, char *p, t_flags *flags);
void		adjust_r_i2(char *s, char *conv, t_flags *flags);
void		adjust_r_i2_space_plus(char *s, char *conv, t_flags *flags);
void		update_for_x(char *s, char *conv, t_flags *flags);
t_flags		*init_flags(void);
void		fill_flags(char *p, t_flags **flags);
void		prepare_flags(char *conv, t_flags *flags);
void		prepare_flags2(char *conv, t_flags *flags);
void		update_flags_r_i(char *p, t_flags *flags);
char		*safemalloc_and_set(ssize_t width, int c);
void		upd_minw_prec_in_flags(char *p, t_flags *flags);
void		get_width_and_precision(char *conv, int *width, int *max_w);
int			width_prec_atoi(char **p);
char		*ft_itoxx(unsigned int nbr);
char		*ft_itou(unsigned int nbr);
void		reverse(char *res, int len);

// debug function
void		printflags(t_flags *flags);
#endif
