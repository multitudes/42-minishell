/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:51:41 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/07 19:50:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include <stdbool.h>

typedef struct s_data t_data;	

bool	handle_history(t_data *data);
bool	add_to_hist_file(const char *input);
int		clear_hist_file(void);
int		print_history(void);
char	*get_history_file_path(void);
void	load_history(void);
bool	sanitize_input(const char *input);

#endif  // HISTORY_H_
