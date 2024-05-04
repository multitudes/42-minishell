/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   darray.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 15:15:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/29 12:34:04 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
The darray is a dynamic array that can store any type of data
*/
t_darray *darray_create(size_t element_size, size_t initial_max)
{
	// here I get a pointer for the struct initiating the memory he needs. The size here is not depending from the contents which will be pointers
	t_darray *array = malloc(sizeof(t_darray));
	if (!array) {
		return NULL;
	}
	// The max can be changed - here I initialize with a sensible size
	array->max = initial_max;
	
	// the contents will be a number of void pointers allocated here
	// each pointer will point to an element of the array on the heap
	array->contents = ft_calloc(initial_max, sizeof(void *));
	
	// the end is zero because the space has been created but no data is in
	array->end = 0;
	// element_size is for ex the size of an int? depending which element I store in my array
	array->element_size = element_size;
	// when it is full I create a new array adding DEFAULT_EXPAND_RATE to the max size
	array->expand_rate = DEFAULT_EXPAND_RATE;
	
	return array;
}

void darray_clear(t_darray * array)
{
	int i = 0;
	// why check the element size? (should be always something line the size of an int if I was storing ints...
	if (array->element_size > 0) {
		i = 0;
		while (i < array->max) 
		{
			if (array->contents[i] != NULL) {
				free(array->contents[i]);
			}
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
void *ft_realloc(void *ptr, size_t size)
{
    void	*new_ptr;

	new_ptr = malloc(size);
	if (!new_ptr)
		return NULL;
	if (ptr == NULL)
        return new_ptr;
    ft_memcpy(new_ptr, ptr, size);
    free(ptr);
    return new_ptr;
}

/*
*/
int darray_resize(t_darray *array, size_t newsize)
{
	void *contents;

	array->max = newsize;
	contents = ft_realloc(array->contents, array->max * sizeof(void *));
	if (!contents) 
		return -1;
	array->contents = contents;
	return 0;
}

/*
*/
int darray_expand(t_darray *array)
{
	size_t	old_max;
	
	old_max = array->max;
	if (darray_resize(array, array->max + array->expand_rate) != 0)
	{
		debug("Failed to expand array to new size: %d", array->max + (int)array->expand_rate);
		return -1;
	}
	ft_memset(array->contents + old_max, 0, array->expand_rate + 1);
	return 0;
}

/*
*/
int darray_contract(t_darray * array)
{
	int new_size;

	if (array->end < (int)array->expand_rate)
		new_size = (int)array->expand_rate;
	else
		new_size = array->end;
	
	return darray_resize(array, new_size + 1);
}

/*
*/
void	darray_destroy(t_darray *array)
{
	if (array) {
		if (array->contents)
			free(array->contents);
		free(array);
	}
}

/*
*/
void	darray_clear_destroy(t_darray *array)
{
	darray_clear(array);
	darray_destroy(array);
}

/*
push takes a already allocated element to add to the array and 
add it to the end of the array
It updates the array end and if the end is bigger than the max it expands the array
The element to add needs to be already allocated on the heap
push will not allocate memory for the element
*/
int darray_push(t_darray *array, void *el)
{
	array->contents[array->end] = el;
	array->end++;
	if (darray_end(array) >= darray_max(array))
		return darray_expand(array);
	else 
		return 0;
}

/*
*/
void *darray_pop(t_darray * array)
{
	if (array->end -1 < 0)
	{
		debug("Attempt to pop from empty array.");
		return NULL;
	}
	void *el = darray_remove(array, array->end - 1);
	array->end--;
	if (darray_end(array) > (int)array->expand_rate && darray_end(array) % array->expand_rate) 
	{
		darray_contract(array);
	}
	return el;
}

/*
access the content by index and assign new element - 
not returning a value
*/
void darray_set(t_darray *array, int i, void *el)
{
	if (i > array->max)
	{
		debug("darray attempt to set past max");
		return ;
	}
	if (i > array->end)
		array->end = i;
	array->contents[i] = el;
}

/*
get the content at index - 
returns a void pointer to the element
*/
void *darray_get(t_darray *array, int i)
{
	if (i >= array->max)
	{
		debug("darray attempt to get past max");
		return NULL;
	}
	return array->contents[i];
}

/*
setting the element at index to null and returning what we removed
*/
void *darray_remove(t_darray * array, int i)
{
	void *el;
	
	el = array->contents[i];
	array->contents[i] = NULL;
	return el;
}

/*
create memory allocation with calloc for a darray
works with constant size elements like ints and floats
for strings I would allocate them and then add them to the array
with darray_push or set
*/
void *darray_new_elem(t_darray *array)
{
	if (array->element_size == 0)
	{
		debug("Can't use darray_new_elem on 0 size darrays.");
		return NULL;
	}
	return ft_calloc(1, array->element_size);
}
