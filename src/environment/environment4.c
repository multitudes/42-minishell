/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:09:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/17 15:58:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "environment.h"
#include "darray.h"
#include "utils.h"
#include "splash_error.h"

#define SIZEOFUINT 8

/*
 * Used in the bubblesort function to return ordered strings.
 * This function is intended to be generic and if i have an array 
 * of ints then size will be size of int or char or whatever type
 * To avoid usinfg an unused large buffer I use SIZEOFCHAR 1 for this project
 */
static void	swappointers(void *a, void *b, size_t size)
{
	char	temp[SIZEOFUINT];

	ft_memcpy(temp, a, size);
	ft_memcpy(a, b, size);
	ft_memcpy(b, temp, size);
}

/*
 * I kept the standard C style signature for my bubble sort function so it can 
 * be used interchangeably with the qsort function pointer.
 */
static void	ft_bubblesort(void *base, size_t nmemb, size_t size, \
							int (*compar)(const void *, const void *))
{
	size_t	i;
	size_t	j;
	char	*arr;
	int		swapped;

	i = 0;
	j = 0;
	arr = base;
	swapped = 1;
	while (i < nmemb - 1 && swapped) 
	{
		swapped = 0;
		j = 0;
		while (j < nmemb - i - 1)
		{
			if (compar(arr + j * size, arr + (j + 1) * size) > 0) 
			{
				swappointers(arr + j * size, arr + (j + 1) * size, size);
				swapped = 1;
			}
			j++;
		}
		i++;
	}
}

/*
 * Used in the bubblesort function to compare strings.
 * Because the darray has a content as void** when I pass the content
 * which an array of strings I need to cast it to a char**.
 * I then compare the strings with ft_strcmp.
 */
static int	cmp(const void *a, const void *b)
{
	const char	*str1;
	const char	*str2;

	str1 = *(const char **)a;
	str2 = *(const char **)b;
	if (!str1)
		return (1);
	if (!str2)
		return (-1);
	return (ft_strcmp(str1, str2));
}

/*
 * Prints the environment variables when `export` command is used:
 * Quoted strings, last command is omitted. 
 */
int	print_env_export(t_darray *env_arr)
{
	t_darray	*export_arr;
	int			status;
	char		*key;
	char		*value;
	int			i;

	i = 0;
	status = 0;
	export_arr = NULL;
	if (copy_env_darray(&export_arr, env_arr))
		return (1);
	ft_bubblesort(export_arr->contents, export_arr->end, sizeof(char *), cmp);
	while (export_arr && export_arr->contents && i < export_arr->end - 1)
	{
		key = get_var_key(export_arr->contents[i++]);
		value = mini_get_env(export_arr, key);
		if (key && 
			(printf("declare -x %s=%c%s%c\n", key, '"', value, '"') < 0))
			status = perror_and_status("printf export", 1);
		free(key);
	}
	darray_clear_destroy(export_arr);
	return (status);
}
