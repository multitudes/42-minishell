/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 19:16:09 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/11 08:14:10 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globbing.h"
#include "splash_error.h"

/*
 * I need to read the files in my directory and check if they match the pattern
 * this function will push a list of files in my darray and return true if it 
 * was successful
*/
bool	match_files_in_directory(t_darray *files, const char *pattern)
{
	t_globbing	gl;
	bool		result;

	result = true;
	if (!init(&gl) || !globbing_loop(files, pattern, gl))
		result = false;
	if (closedir(gl.dirp))
		perror_and_null("closedir");
	return (result);
}

/*
 * This recursive function will check if the pattern matches the file name
 * it will return true if it does
*/
bool	is_glob_match(const char *pattern, const char *file_name)
{
	bool result;

	result = false;
	if (*pattern == '\0')
		result = *file_name == '\0';
	else if (*pattern == '*')
	{
		if (is_glob_match(pattern + 1, file_name))
			result = true;
		else
			result = *file_name != '\0' \
						&& is_glob_match(pattern, file_name + 1);
	}
	else 
		result = *pattern == *file_name \
					&& is_glob_match(pattern + 1, file_name + 1);
	return (result);
}
