/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:37:49 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/29 12:59:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// needed for the tests - leave it here
#ifdef __cplusplus
extern "C" {
#endif

#include "minishell.h"

/*
forward declaration
*/
typedef struct s_mini_data t_mini_data;


/*
enviromnemt list:
- from the system I get the environ variable as a char **environ
	and I save it in my data struct
*/

bool update_env(t_mini_data *data, const char *key, const char *value);
void print_env(t_mini_data *data);
char *mini_get_env(t_mini_data *data, const char *key);

#ifdef __cplusplus
}
#endif

#endif