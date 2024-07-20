/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:11:53 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:11:56 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
void ft_putendl_fd(char *s, int fd);

 Parameters
 s:  The string to output.
 fd:  The file descriptor on which to write.
 Outputs the string ’s’ to the given file descriptor
 followed by a newline.
 */
void	ft_putendl_fd(char *s, int fd)
{
	int	count;

	if (s == NULL)
		return ;
	count = ft_strlen(s);
	write(fd, s, count);
	write(fd, NEWLINE, 1);
}
