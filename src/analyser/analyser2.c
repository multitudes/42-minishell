/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 09:22:07 by rpriess           #+#    #+#             */
/*   Updated: 2024/06/17 15:47:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "analyser.h"


void	set_flags(t_list *tokenlist, t_exp_flags *flags)
{
	if (get_token_type(tokenlist) == VAR_EXPANSION)
		flags->dollar_exp_front = true;
	if (ft_strchr(get_token_lexeme(tokenlist), '=') && flags->equal_status == 0)
		flags->equal_status = 1;
	else if (ft_strchr(get_token_lexeme(tokenlist), '=') && flags->equal_status == 1)
		flags->equal_status = 2;
}

void	reset_flags(t_exp_flags *flags)
{
	flags->equal_status = 0;
	flags->dollar_exp_front = false;
}
