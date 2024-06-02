/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 12:14:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globbing.h"
#include "minishell.h"

/*
I need to read the files in my directory and check if they match the pattern
this function will return a list of files in my directory
*/

int	get_files_in_directory(char **files) 
{
	(void)files;
    DIR *dirp;
    struct dirent *dir;

    dirp = opendir(".");
    if (!dirp)
	{
		perror("opendir");
		return (1);
	} 
	errno = 0;
	dir = readdir(dirp);
	while ((dir != NULL) && (errno == 0))
	{
		printf("%s\n", dir->d_name);
		
		dir = readdir(dirp);
	}
	if (errno)
	{
		perror("readdir");
		return (1);
	}
	if (closedir(dirp))
	{
		perror("closedir");
		return (1);
	}
	return (0);
}