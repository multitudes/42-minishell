/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 14:49:09 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:18:17 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS

void ft_putstr_fd(char *s, int fd);

 Parameters
 s:  The string to output.
 fd:  The file descriptor on which to write.
 Outputs the string ’s’ to the given file
 descriptor.
 */
void	ft_putstr_fd(char *s, int fd)
{
	int	count;

	if (s == NULL)
		return ;
	count = (int)ft_strlen(s);
	write(fd, s, count);
}
