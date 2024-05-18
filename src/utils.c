/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:37:46 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/18 16:30:55 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	total_length = strlen(s1) + strlen(s2) + strlen(s3) + 1;
	result = malloc(total_length);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, total_length);
	ft_strlcat(result, s2, total_length);
	ft_strlcat(result, s3, total_length);
	return (result);
}
