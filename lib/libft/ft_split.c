/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 17:03:15 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/22 19:39:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define IN   1  /* inside a word */
#define OUT  0  /* outside a word */

#include <stdio.h>
/*
SYNOPSIS

char **ft_split(char const *s, char c);

 s:  The string to be split.
 c:  The delimiter character.
 Return value
 The array of new strings resulting from the split.
 NULL if the allocation fails.
 Allocates (with malloc(3)) and returns an array
 of strings obtained by splitting ’s’ using the
 character ’c’ as a delimiter.  The array must end
 with a NULL pointer.
 allowed are malloc, free
 */
// inspired by UNIX
// I use a char** str because the string *str will be changed 
// in the func
//*ft_substr(char const *s, unsigned int start, size_t len)
static char	*ft_strsep(char **str, char *delim)
{
	char	*s;
	int		c;
	int		i;
	char	*tok;	

	if (*str == NULL || str == NULL)
		return (NULL);
	i = 0;
	s = *str;
	while (*s == *delim)
		s++;
	tok = s;
	while (1)
	{
		c = *s++;
		i++;
		if (c == *delim || c == '\0')
		{
			if (c != 0)
				i--;
			*str = s;
			return (ft_substr(tok, 0, i));
		}
	}
}

// Counting the words divided by the char c 
static int	my_word_count(const char *s, char c)
{
	int		count;
	int		state;

	state = OUT;
	count = 0;
	while (*s != '\0' && (s != NULL))
	{
		if (*s == c)
			state = OUT;
		else if (state == OUT)
		{
			state = IN;
			count++;
		}
		s++;
	}
	return (count);
}

static void	*ft_free(char **arr, int i)
{
	while (i >= 0)
	{
		if (arr[i] != NULL)
			free(arr[i]);
		i--;
	}
	free(arr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		count;
	char	**arr;

	if (s == NULL)
		return (NULL);
	i = 0;
	count = 0;
	count = my_word_count(s, c);
	arr = malloc(sizeof(char *) * (count + 1));
	if (arr == NULL)
		return (NULL);
	while (i < count)
	{
		arr[i] = ft_strsep((char **)&s, &c);
		if (arr[i] == NULL)
			return (ft_free(arr, i--));
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

// #include <stdio.h>
// void freeTab(char * * tab)
// {
// 	for (int i = 0; tab[i] != NULL; ++i)
// 	{
// 		printf("freeing in arr[%d] ===> %s\n", i, tab[i]);
// 		free(tab[i]);
// 	}
// 	free(tab);
// }
// int main()
// {
// 	char **res = ft_split("hello!zzzzzzzz", 'z');
// 	printf("==== %s \n",res[0]);
// 	printf("==== %s \n",res[1]);
// 	// printf("==== %s \n",res[2]);
// 	// printf("==== %s \n",res[3]);
// 	// printf("==== %s \n",res[4]);
// 	// printf("==== %s \n",res[5]);
// 	// printf("==== %s \n",res[6]);
// 	// printf("==== %s \n",res[7]);
// 	// printf("==== %s \n",res[8]);
// 	// printf("==== %s \n",res[9]);
// 	// printf("==== %s \n",res[10]);
// 	// printf("==== %s \n",res[11]);
// 	// printf("==== %s \n",res[12]);
// 	// printf("==== %s \n",res[13]);
// 	// char **res2 = ft_split("hello!zzzzzzzz", 'z');
// 	// 	printf("==== %s \n",res2[0]);
// 	// char test[45] = "--1-2--3---4----5-----42";
// 	// char **res = ft_split(test, '-');
// 	// printf("==== %s-\n",res[0]);
// 	// printf("==== %s \n",res[1]);
// 	// printf("==== %s \n",res[2]);
// 	// printf("==== %s \n",res[3]);
// freeTab(res);
// }
