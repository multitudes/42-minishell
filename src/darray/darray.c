/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   darray.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 15:15:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 13:26:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"
#include <libft.h>

/*
 * Returns a new darray with the given element size and initial max size
 * otherwise returns NULL
 * The darray is a dynamic array that can store any type of data
 * when it is full I create a new array adding DEFAULT_EXPAND_RATE 
 * to the max size
 * element_size is for ex the size of an int or depending which 
 * element I store in my array 
 * If adding ints they need to be preallocated with darray_new
 * strings can be malloced and added with darray_push
*/
t_darray	*darray_create(size_t element_size, size_t initial_max)
{
	t_darray	*array;

	array = malloc(sizeof(t_darray));
	if (!array)
		return (NULL);
	array->max = initial_max;
	array->contents = ft_calloc(initial_max, sizeof(void *));
	array->end = 0;
	array->element_size = element_size;
	array->expand_rate = DEFAULT_EXPAND_RATE;
	return (array);
}

/*
 * the content is an array of pointers. Frees the content pointed to.
 * sets every pointer to NULL from zero to max
 * and sets the end to 0
*/
void	darray_clear(t_darray *array)
{
	int	i;

	i = 0;
	if (array->element_size > 0)
	{
		i = 0;
		while (i < array->max)
		{
			if (array->contents[i] != NULL)
				free(array->contents[i]);
			array->contents[i] = NULL;
			i++;
		}
	}
	array->end = 0;
}

/*
 * reimplementation of the realloc function but we need the extra param.
 * In C, if you call realloc with a null pointer as the first argument, 
 * it behaves exactly like malloc. It simply allocates a new block of 
 * memory of the specified size. It doesn't need to 
 * copy or free anything because there's no existing block of memory.
*/
void	*ft_realloc(void *ptr, size_t size, size_t old_size)
{
	void	*new_ptr;

	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	if (ptr == NULL)
		return (new_ptr);
	if (size <= old_size)
		ft_memcpy(new_ptr, ptr, size);
	else
		ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}

/*
 * Called by darray_expand it will reallocate the old array and 
 * returns -1 in case of mem failure and keep the old size
*/
int	darray_resize(t_darray *array, size_t newsize, size_t old_size)
{
	void	*contents;

	array->max = newsize;
	contents = ft_realloc(array->contents, array->max * sizeof(void *), \
	old_size * sizeof(void *));
	if (!contents)
	{
		array->max = old_size;
		return (-1);
	}
	array->contents = contents;
	return (0);
}

/*
 * In our darray implementation the expansion is additive and stored 
 * in the expand_rate
*/
int	darray_expand(t_darray *array)
{
	size_t	old_max;

	old_max = array->max;
	if (darray_resize(array, array->max + array->expand_rate, old_max) != 0)
	{
		write(2, "Failed to expand array", 23);
		return (-1);
	}
	ft_memset(array->contents + old_max, 0, array->expand_rate + 1);
	return (0);
}
