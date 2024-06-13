/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/04 14:00:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globbing.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
/*
I need to read the files in my directory and check if they match the pattern
this function will push a list of files in my darray and return true if it was successful
*/

bool	match_files_in_directory(t_darray *files, const char *pat) 
{
    DIR *dirp;
	char cwd[PATH_MAX];
    struct dirent *dir_entry;
	struct stat path_stat;
	char *full_path;

    dirp = opendir(".");
    if (!dirp)
		return (false_and_perr("opendir"));
	errno = 0;
	dir_entry = readdir(dirp);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		if (closedir(dirp))
			perror("closedir");
		return (false);
	}
	debug("current working directory: %s\n", cwd);
	while (dir_entry && !errno)
	{
		debug("number of files in directory: %d\n", files->end);
		debug("-%s-", dir_entry->d_name);
		full_path = ft_strjoin3(cwd, "/", dir_entry->d_name);
		if (!full_path)
			return (false);
		// free
		debug("full path: %s\n", full_path);

		
		if (stat(full_path, &path_stat) == 0)
		{
			if (S_ISREG(path_stat.st_mode))
			{
				// debug("file considered: %s -----------------> ", dir->d_name);
				if (is_glob_match(pat, dir_entry->d_name))
					darray_push(files, ft_strdup(dir_entry->d_name));
			}
		}
		else
			perror("stat");

		free(full_path);
		errno = 0;
		dir_entry = readdir(dirp);
	}
	if (errno)
		perror("readdir");
	if (closedir(dirp))
		false_and_perr("closedir");
	return (true);
}

bool	is_glob_match(const char *pat, const char *file_name)
{
	debug("MATCHING pattern: %s file_name: %s", pat, file_name);
    if (*pat == '\0')
        return (*file_name == '\0');
    if (*pat == '?')
        return (*file_name != '\0' && is_glob_match(pat + 1, file_name + 1));
    if (*pat == '*') 
	{
        if (is_glob_match(pat + 1, file_name))
		{
			return 1;
		}
        return (*file_name != '\0' && is_glob_match(pat, file_name + 1));
    }
    return (*pat == *file_name && is_glob_match(pat + 1, file_name + 1));
}
