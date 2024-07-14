/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:37:46 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 11:41:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <libft.h>
#include "splash_error.h"
#include "scanner.h"
#include <stdbool.h>

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
This function joins three strings together.
The first string should not be NULL but could be an empty string.
*/
char	*ft_strjoin3(const char *s1, const char	*s2, const char	*s3)
{
	char	*result;
	size_t	total_length;

	total_length = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	result = malloc(total_length);
	if (!result)
		return (perror_and_null("malloc ft_strjoin3"));
	ft_strlcpy(result, s1, total_length);
	ft_strlcat(result, s2, total_length);
	ft_strlcat(result, s3, total_length);
	return (result);
}

/*
Counts the number of occurances of a char c in a string.
*/
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
Returns true if all chars in str are an ascii digit.
Initial char can be '+' or '-'.
Returns falls if str is NULL.
*/
bool	ft_isnumstring(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (false);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}
