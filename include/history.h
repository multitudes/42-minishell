/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:51:41 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/10 11:52:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include <stdbool.h>

typedef struct s_data t_data;	

bool	handle_history(t_data *data);
bool	add_to_hist_file(const char *input, t_darray *env_arr);
int		clear_hist_file(t_darray *env_arr);
int		print_history(t_darray *env_arr);
char	*get_history_file_path(t_darray *env_arr);
void	load_history(t_darray *env_arr);
bool	sanitize_input(const char *input);

#endif  // HISTORY_H_
