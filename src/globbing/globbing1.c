/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 19:16:09 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/06 14:39:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globbing.h"
#include "splash_error.h"

/*
 * I need to read the files in my directory and check if they match the pattern
 * this function will push a list of files in my darray and return true if it 
 * was successful
*/
bool	match_files_in_directory(t_darray *files, const char *pat)
{
	t_globbing	gl;
	bool		result;

	result = true;
	if (!init(&gl) || !globbing_loop(files, pat, gl))
		result = false;
	if (closedir(gl.dirp))
		false_and_perr("closedir");
	return (result);
}

/*
 * This recursive function will check if the pattern matches the file name
 * it will return true if it does
*/
bool	is_glob_match(const char *pat, const char *file_name)
{
	if (*pat == '\0')
		return (*file_name == '\0');
	if (*pat == '?')
		return (*file_name != '\0' && is_glob_match(pat + 1, file_name + 1));
	if (*pat == '*')
	{
		if (is_glob_match(pat + 1, file_name))
			return (true);
		return (*file_name != '\0' && is_glob_match(pat, file_name + 1));
	}
	return (*pat == *file_name && is_glob_match(pat + 1, file_name + 1));
}
