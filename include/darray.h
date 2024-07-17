/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   darray.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 15:27:54 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/10 17:31:05 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DARRAY_H
# define DARRAY_H

# include "stddef.h"

# define DEFAULT_EXPAND_RATE 300

/*
   A dynamic array is simply an array of void ** pointers that’s 
   pre-allocated in one shot and that point at the data.
   The size_t data type in C is an unsigned integer type used to 
   represent the size of objects in bytes. It is defined in the stddef. 
   h header and is commonly used to represent the size of arrays, 
   memory blocks, and strings.
   */
typedef struct s_darray
{
	int						end;
	int						max;
	size_t					element_size;
	size_t					expand_rate;
	void					**contents;
}							t_darray;

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif

void *ft_realloc(void *ptr, size_t size, size_t old_size);
t_darray	*darray_create(size_t element_size, size_t initial_max);
void		darray_destroy(t_darray *array);
int			darray_resize(t_darray *array, size_t newsize, size_t old_size);
void		darray_clear(t_darray *array);
int			darray_expand(t_darray *array);
int			darray_contract(t_darray *array);
int			darray_push(t_darray *array, void *el);
void		*darray_pop(t_darray *array);
int			darray_clear_destroy(t_darray *array);
void		*darray_remove(t_darray *array, int i);
void		*darray_remove_and_shift(t_darray *array, int i);
void		*darray_new_elem(t_darray *array);
void		darray_set(t_darray *array, int i, void *el);
void		*darray_get(t_darray *array, int i);

#  ifdef __cplusplus

}
#  endif

# endif
