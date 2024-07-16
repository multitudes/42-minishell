/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freedata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:34:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 12:37:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"

/*
 * util function
 * it makes sense to collect what we malloc in our data struct
 * because it is easier to free at the end or when we need to exit
 * tokenlist is freed in the loop with the ast
*/
void	free_data(t_data **data)
{
	if (data == NULL || *data == NULL)
		return ;
	darray_clear_destroy((*data)->env_arr);
	free((void *)((*data)->homepath));
	(*data)->homepath = NULL;
	free(*data);
	*data = NULL;
}
