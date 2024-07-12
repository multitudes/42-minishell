/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 11:41:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/12 11:52:50 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

#include "darray.h"
#include "minishell.h"

bool	init_data(t_data **data);
bool	init_data2(t_data **data);
void	shlvl_init(t_data *data);
bool	init_env_darray(t_darray **env_arr);

#endif  // INIT_H_