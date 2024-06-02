/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:37 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 19:31:14 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBBING_H
# define GLOBBING_H

# ifdef __cplusplus

extern "C" {
# endif

# include "minishell.h"

bool	match_files_in_directory(t_darray *files, char *pat);
bool	is_glob_match(char *pat, char *file_name);

#  ifdef __cplusplus

}
#  endif

#endif