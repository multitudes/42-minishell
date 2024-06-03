/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   darray3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:40:09 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/03 11:41:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"
#include "minishell.h"

/*
get the content at index - 
returns a void pointer to the element
*/
void	*darray_get(t_darray *array, int i)
{
	if (i >= array->max)
	{
		debug("darray attempt to get past max");
		return (NULL);
	}
	return (array->contents[i]);
}

/*
setting the element at index to null
and returning what we removed (to free)
*/
void	*darray_remove(t_darray *array, int i)
{
	void	*el;

	debug("darray_remove");
	el = array->contents[i];
	array->contents[i] = NULL;
	return (el);
}

/*
removing the element at index, moving the subsequent elements up
and returning the element that was removed (to free).
add call to contract_array ?
*/
void	*darray_remove_and_shift(t_darray *array, int i)
{
	void	*el;

	debug("darray_remove_and_shift");
	el = darray_remove(array, i);
	while (i < array->end)
	{
		array->contents[i] = array->contents[i + 1];
		i++;
	}
	array->end--;
	return (el);
}

/*
create memory allocation with calloc for a darray
works with constant size elements like ints and floats
for strings I would allocate them and then add them to the array
with darray_push or set
*/
void	*darray_new_elem(t_darray *array)
{
	if (array->element_size == 0)
	{
		debug("Can't use darray_new_elem on 0 size darrays.");
		return (NULL);
	}
	return (ft_calloc(1, array->element_size));
}

/*
*/
int	darray_contract(t_darray *array)
{
	int	new_size;

	if (array->end < (int)array->expand_rate)
		new_size = (int)array->expand_rate;
	else
		new_size = array->end;
	return (darray_resize(array, new_size + 1, array->max));
}
