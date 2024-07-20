/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:54:47 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/20 10:57:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash_error.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h> // remove and replace printf with write_data or ft_write

/*
 * Executes builtin 'pwd' command.
 */
uint8_t	execute_pwd_builtin(void)
{
	char	cur_dir[PATH_MAX];

	if (!getcwd(cur_dir, PATH_MAX))
		return (perror_and_status("pwd: getcwd", 1));
	if (printf("%s\n", cur_dir) < 0)
		return (perror_and_status("pwd: printf", 1));
	return (0);
}
