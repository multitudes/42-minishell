/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:37 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/02 17:23:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBBING_H
# define GLOBBING_H

# ifdef __cplusplus

extern "C" {
# endif

#include <dirent.h>
#include <sys/stat.h>
// this below is definitely needed by macos
#include <limits.h>
#include "darray.h"
#include <stdbool.h>
#include "libft.h"

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

bool	init(t_globbing *gl);
bool 	globbing_loop(t_darray *files, const char *pat, t_globbing gl);
bool	match_files_in_directory(t_darray *files, const char *pat);
bool	is_glob_match(const char *pat, const char *file_name);
t_list	*create_globbing_tokenlist(t_darray *files);
bool	is_glob_match(const char *pat, const char *file_name);

#  ifdef __cplusplus

}
#  endif

#endif
