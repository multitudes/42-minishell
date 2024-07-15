/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 12:45:51 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 12:04:57 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FD_H
# define FD_H

# include "splash.h"

void	init_fds(t_data *data);
void	save_fds(t_data *data);
void	restore_fds(t_data *data);

#endif  // FD_H_