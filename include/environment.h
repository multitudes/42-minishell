/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:37:49 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/22 16:02:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif

#include "minishell.h"

/*
forward declaration
*/
typedef struct s_data t_data;
typedef struct s_darray t_darray;
/*
enviromnemt list:
- from the system I get the environ variable as a char **environ
	and I save it in my data struct
*/

bool	update_env(t_data *data, const char *key, const char *value);
int		print_env(t_darray *env_array);
char	*mini_get_env(t_data *data, const char *key);
char	*ft_strjoin3(const char *key, const char *mid, const char *value);

#  ifdef __cplusplus

}
#  endif

# endif