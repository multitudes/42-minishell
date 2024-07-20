/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 14:40:08 by lbrusa            #+#    #+#             */
/*   Updated: 2023/11/21 18:18:23 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS

void ft_putchar_fd(char c, int fd);

 c:  The character to output.
 fd:  The file descriptor on which to write.
 Outputs the character ’c’ to the given file
 descriptor.
 write
 */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
