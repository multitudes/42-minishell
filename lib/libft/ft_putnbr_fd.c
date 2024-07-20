/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:28:46 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:12:09 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
void ft_putnbr_fd(int n, int fd);

 Outputs the integer ’n’ to the given file
 descriptor.
 */
void	ft_putnbr_fd(int n, int fd)
{
	char	*temp;
	size_t	c;

	c = 0;
	temp = NULL;
	temp = ft_itoa(n);
	c = ft_strlen(temp);
	write(fd, temp, c);
	if (temp != NULL)
		free(temp);
}
