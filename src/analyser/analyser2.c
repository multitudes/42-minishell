/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 09:22:07 by rpriess           #+#    #+#             */
/*   Updated: 2024/06/17 15:18:00 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "analyser.h"


void	set_flags(t_list *tokenlist, t_exp_flags *flags)
{
	if (get_token_type(tokenlist) == VAR_EXPANSION)
		flags->dollar_exp_front = true;
	if (ft_strchr(get_token_lexeme(tokenlist), '=') && flags->pos_equal_sep == 0)
		flags->pos_equal_sep = 1;
	else if (ft_strchr(get_token_lexeme(tokenlist), '=') && flags->pos_equal_sep == 1)
		flags->pos_equal_sep = 2;
}

void	reset_flags(t_exp_flags *flags)
{
	flags->pos_equal_sep = 0;
	flags->dollar_exp_front = false;
}
