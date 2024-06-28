/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:37 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/28 12:03:25 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBBING_H
# define GLOBBING_H

# ifdef __cplusplus

extern "C" {
# endif

#include "darray.h"
#include <stdbool.h>

bool	match_files_in_directory(t_darray *files, const char *pat);
bool	is_glob_match(const char *pat, const char *file_name);
bool 	globbing_loop(t_darray *files, const char *pat, t_globbing gl);

#  ifdef __cplusplus

}
#  endif

#endif
