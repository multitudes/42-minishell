/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:32:28 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:14:08 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
      #include <string.h>
      char *strrchr(const char *s, int c);

 The strrchr() function is identical to strchr(),
 except it locates the last occurrence of c.
*/
char	*ft_strrchr(const char *s, int c)
{
	int		i;

	if (s == NULL)
		return (NULL);
	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == (unsigned char)c)
			return ((char *)s + i);
		i--;
	}
	return (NULL);
}

//int main()
//{
//	char s1[] = "tripouille";
//	printf("is this NULL? %s \n",ft_strrchr(s1, 'z'));
//	if ((char*)ft_strrchr(s1, 'z') == (char*)NULL)
//		printf("null after all.. \n");
//	printf("is this NULL? %s \n",(char*)NULL);
//	return (0);
//}
//char arr[BUFSIZ];
//
//bytes_read = read(fd, arr, BUFSIZ))
//while (bytes_read  > 0)
//{
//	if (write(1, arr, bytes_read) != bytes_read)
//		... write(1, "write error on file ...",24;
//	bytes_read = read(fd, arr, BUFSIZ))
//}
