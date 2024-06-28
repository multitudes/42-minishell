/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:37 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/28 12:43:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBBING_H
# define GLOBBING_H

# ifdef __cplusplus

extern "C" {
# endif

#include <dirent.h>
#include <sys/stat.h>
#include "darray.h"
#include <stdbool.h>

/*
** convenience struct used to read from the directory index for globbing
*/
typedef struct 		s_globbing
{
    DIR 			*dirp;
	char 			cwd[PATH_MAX];
    struct dirent	*dir_entry;
	struct stat 	path_stat;
	char 			*full_path;
}					t_globbing;

bool	match_files_in_directory(t_darray *files, const char *pat);
bool	is_glob_match(const char *pat, const char *file_name);
bool 	globbing_loop(t_darray *files, const char *pat, t_globbing gl);

#  ifdef __cplusplus

}
#  endif

#endif
