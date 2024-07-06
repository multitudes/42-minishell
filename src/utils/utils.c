/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:37:46 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/06 14:39:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <libft.h>
#include "splash_error.h"
#include "scanner.h"


/*
 The strndup() function is similar to ft_strdup, but copies at most n bytes. 
 If s is longer than n, only n bytes are copied,
 and a terminating null byte ('\0') is added.
 The duplicate string is allocated with malloc, which needs to be freed.
*/
char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*new_str;

	if (s == NULL)
		return (NULL);
	if (ft_strlen(s) <= n)
		return (ft_strdup(s));
	else
		new_str = malloc(n + 1);
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		new_str[i] = s[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

/*
util . otherwise non ascii chars like ∂ will wreac havoc and the read function
will stop reading the file.
inline functions are typically defined in header files (cant do it here)
*/
int	ft_isascii(const int c)
{
	return (c >= 0 && c <= 127);
}

/*
This function will join three strings together
This pattern is often used 
(see man page for strlcpy and strlcat)

size_t n = ft_strlcpy(result, s1, total_length)
if (n > total_length)
	return (NULL); 

but since in this function I know the total length of the string
because I allocate it such checks are not needed
*/
char	*ft_strjoin3(const char *s1, const char	*s2, const char	*s3)
{
	char	*result;
	size_t	total_length;

	total_length = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	result = malloc(total_length);
	if (!result)
		return (null_on_err("malloc ft_strjoin3"));
	ft_strlcpy(result, s1, total_length);
	ft_strlcat(result, s2, total_length);
	ft_strlcat(result, s3, total_length);
	return (result);
}


int	count_char_in_str(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

/*
 * This function will replace the node with the new list
 * it will return nothing but will modify the tokenlist
 * params: 
 * head - the head of the tokenlist - will be modified only if the newlist
 * is the first node
 * node - the node to be replaced - it will be freed also
 * newlist - the new list to replace the node
*/
void	replace_node_with_newlist(t_list **node, t_list *newlist)
{
	t_list *prev;
	t_list *next;
	t_list *last;
	t_list *tmp;
	
	tmp = *node;
	prev = (*node)->prev;
	next = (*node)->next;
	if (prev)
		prev->next = newlist;
	else
		tmp = newlist;
	newlist->prev = prev;
	last = ft_lstlast(newlist);
	last->next = next;
	if (next)
		next->prev = last;
	ft_lstdelone(*node, free_tokennode);
	if (!prev)
		*node = tmp;
}