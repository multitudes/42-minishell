/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   darray.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 15:15:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 13:54:08 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
The darray is a dynamic array that can store any type of data
element size isto make it more generic but it is not used maybe?
could be refactored later, but usually because of memory alignment
we need to know the size of the elements we store to calculate the
memory needed for the array...
when it is full I create a new array adding DEFAULT_EXPAND_RATE 
to the max size
element_size is for ex the size of an int? depending which 
element I store in my array

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
}

/*
In C, if you call realloc with a null pointer as the first argument, 
it behaves exactly like malloc. It simply allocates a new block of 
memory of the specified size. It doesn't need to 
copy or free anything because there's no existing block of memory.
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
*/
int	darray_resize(t_darray *array, size_t newsize, size_t old_size)
{
	void	*contents;

	array->max = newsize;
	contents = ft_realloc(array->contents, array->max * sizeof(void *), \
	old_size * sizeof(void *));
	if (!contents)
		return (-1);
	array->contents = contents;
	return (0);
}

/*
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
