/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 19:29:29 by lbrusa           ###   ########.fr       */
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
this function will push a list of files in my darray and return true if it was successful
*/

bool	match_files_in_directory(t_darray *files, char *pat) 
{
    DIR *dirp;
	char cwd[PATH_MAX];
    struct dirent *dir;
	struct stat path_stat;

    dirp = opendir(".");
    if (!dirp)
	{
		perror("opendir");
		return (false);
	} 
	errno = 0;
	dir = readdir(dirp);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		return (false);
	}
	debug("current working directory: %s\n", cwd);
	while ((dir != NULL) && (errno == 0))
	{
		debug("number of files in directory: %d\n", files->end);
		debug("-%s-", dir->d_name);
		char *full_path = ft_strjoin3(cwd, "/", dir->d_name);
		if (!full_path)
		{
			perror("ft_strjoin3");
			return (false);
		}
		// free
		debug("full path: %s\n", full_path);

		
		if (stat(full_path, &path_stat))
		{
			perror("stat");
			return (false);
		}
		if (S_ISREG(path_stat.st_mode))
		{
			debug("file added: %s -----------------> ", dir->d_name);
			if (is_glob_match(pat, dir->d_name))
				darray_push(files, ft_strdup(dir->d_name));
		}
		free(full_path);
		dir = readdir(dirp);
	}
	if (errno)
	{
		perror("readdir");
		if (closedir(dirp))
		{
			perror("closedir");
			return (false);
		}
		return (false);
	}
	if (closedir(dirp))
	{
		perror("closedir");
		return (false);
	}

	return (true);
}

bool	is_glob_match(char *pat, char *file_name)
{
	(void)pat;
	(void)file_name;

	return (true);
}