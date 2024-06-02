/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:37 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 14:01:02 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBBING_H
# define GLOBBING_H

# ifdef __cplusplus

extern "C" {
# endif

# include "minishell.h"

int	get_files_in_directory(t_darray *files);


#  ifdef __cplusplus

}
#  endif

#endif