/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 14:20:37 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globbing.h"
#include "minishell.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>



/*
I need to read the files in my directory and check if they match the pattern
this function will return a list of files in my directory
*/

int	get_files_in_directory(t_darray *files) 
{
    DIR *dirp;
	char cwd[1024];
    struct dirent *dir;
	struct stat path_stat;

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
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd");
			return (1);
		}
		stat(cwd, &path_stat);
		if (S_ISREG(path_stat.st_mode))
			darray_push(files, ft_strdup(dir->d_name));
		dir = readdir(dirp);
	}
	if (errno)
	{
		perror("readdir");
		if (closedir(dirp))
		{
			perror("closedir");
			return (1);
		}
		return (1);
	}
	if (closedir(dirp))
	{
		perror("closedir");
		return (1);
	}
	return (0);
}