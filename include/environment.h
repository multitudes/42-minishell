/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:37:49 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 08:57:54 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif

# include <stdbool.h>
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

bool	update_env(t_darray *env_arr, const char *key, const char *value);
int		copy_env_darray(t_darray **new_array, t_darray *source_arr);
int		delete_env_entry(t_darray *env_arr, char *key);
int		get_var_index(t_darray *env_arr, const char *key);
char	*get_var_key(const char *variable);
char	*get_var_value(const char *variable);
int		print_env(t_darray *env_array);
int		print_env_export(t_darray *env_arr);
char	*mini_get_env(t_darray *env_array, const char *key);

#  ifdef __cplusplus

}
#  endif

# endif
