/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 09:22:07 by rpriess           #+#    #+#             */
/*   Updated: 2024/06/17 09:25:12 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "analyser.h"

void	reset_flags(t_exp_flags *flags)
{
	flags->pos_equal_sep = 0;
	flags->dollar_exp_front = false;
}
