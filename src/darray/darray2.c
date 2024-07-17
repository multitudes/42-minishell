/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   darray2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:35:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/17 18:38:02 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"
#include <stdlib.h>
#include "debug.h"
/*
 * free the internal content array and deallocate the darray struct
*/
void	darray_destroy(t_darray *array)
{
	if (array)
	{
		if (array->contents)
			free(array->contents);
		free(array);
	}
}

/*
 * clear will free the content of the pointers in the  
 * internal content array to a zero. destroy will free the 
 * internal content array and deallocate the darray struct
*/
int	darray_clear_destroy(t_darray *array)
{
	darray_clear(array);
	darray_destroy(array);
	return (1);
}

/*
push takes a already allocated element to add to the array and 
add it to the end of the array
It updates the array end and if the end is bigger than the max it 
expands the array.
The element to add needs to be already allocated on the heap
push will not allocate memory for the element
*/
int	darray_push(t_darray *array, void *el)
{
	array->contents[array->end] = el;
	array->end++;
	if (array->end >= array->max)
		return (darray_expand(array));
	else
		return (0);
}

/*
*/
void	*darray_pop(t_darray *array)
{
	void	*element;

	if (array->end -1 < 0)
		return (NULL);
	element = darray_remove(array, array->end - 1);
	array->end--;
	if (array->end > (int)array->expand_rate && array->end \
	% array->expand_rate)
		darray_contract(array);
	return (element);
}

/*
access the content by index and assign new element - 
not returning a value
*/
void	darray_set(t_darray *array, int i, void *el)
{
	if (i > array->max)
		return ;
	if (i > array->end)
		array->end = i;
	array->contents[i] = el;
}
