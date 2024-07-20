/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsflags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:21:44 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/17 19:55:56 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*

*/
void	adjust_r_i2_space_plus(char *s, char *conv, t_flags *flags)
{
	ft_memcpy(s + 1 + (flags->prec - flags->len), conv, flags->len);
	if (!flags->space)
		s[0] = '+';
	if (flags->space)
		s[0] = ' ';
}

/*

*/
void	adjust_r_i2(char *s, char *conv, t_flags *flags)
{
	if ((flags->plus || flags->space) && conv[0] != '-' && (!flags->unsgned))
		adjust_r_i2_space_plus(s, conv, flags);
	else if (conv[0] == '-')
	{
		flags->start = 1 + (flags->prec - flags->len + 1);
		ft_memcpy(s + flags->start, conv + 1, flags->len - 1);
		s[0] = '-';
		if ((flags->fw > flags->prec) && flags->prec >= flags->len)
			ft_memset(s + flags->prec + 1, ' ', flags->fw - flags->prec - 1);
		if ((flags->fw > flags->prec) && flags->len > flags->prec)
			ft_memset(s + flags->len, ' ', flags->fw - flags->len);
	}
	else if (!(flags->len == 1 && conv[0] == '0' && flags->dot))
	{
		if (flags->prec > flags->len)
			ft_memcpy(s + (flags->prec - flags->len), conv, flags->len);
		else
			ft_memcpy(s, conv, flags->len);
		if ((flags->fw > flags->prec) && flags->prec >= flags->len)
			ft_memset(s + flags->prec, ' ', flags->fw - flags->prec);
	}
	else if (!(conv[0] == '0'))
		ft_memcpy(s, conv, flags->len);
	update_for_x(s, conv, flags);
}

/*

*/
void	adjust_r_i(char *s, char *conv, t_flags *flags)
{
	if (flags->prec == 0 && flags->len == 1 && conv[0] == '0' && flags->dot)
	{
		ft_memset(s, ' ', flags->fw);
		if (flags->minw == 0)
		{
			s[flags->fw - 1] = 0;
			flags->fw = 0;
		}
	}
	else if ((flags->fw > flags->prec) && flags->prec >= flags->len \
	&& conv[0] == '0' && flags->dot)
		ft_memset(s + flags->prec, ' ', flags->fw - flags->prec);
	if (conv[0] == '-')
	{
		if ((flags->prec - flags->len) < 0)
			flags->prec = flags->len - 1;
	}
	else if ((flags->prec - flags->len) < 0)
		flags->prec = flags->len;
	adjust_r_i2(s, conv, flags);
}

/*

*/
void	fill_flags2(char *conv, t_flags **flags)
{
	while (ft_strchr("+-0 ", *conv) != NULL && *conv)
	{
		if (*conv == '+')
			(*flags)->plus = 1;
		if (*conv == '0')
		{
			(*flags)->zero = 1;
			(*flags)->pad = '0';
		}
		if (*conv == '-')
			(*flags)->minus = 1;
		if (*conv == ' ')
			(*flags)->space = 1;
		if (*conv == '*')
			(*flags)->star += 1;
		if (*conv == '.')
			(*flags)->dot = 1;
		conv++;
	}
	upd_minw_prec_in_flags(conv, *flags);
}

void	fill_flags(char *p, t_flags **flags)
{
	while (ft_strchr("+-0# ", *p) != NULL && *p)
	{
		if (*p == '+')
			(*flags)->plus = 1;
		if (*p == '0')
		{
			(*flags)->zero = 1;
			(*flags)->pad = '0';
		}
		if (*p == '-')
			(*flags)->minus = 1;
		if (*p == ' ')
			(*flags)->space = 1;
		if (*p == '*')
			(*flags)->star += 1;
		p++;
	}
	upd_minw_prec_in_flags(p, *flags);
}
