/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:51:41 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/05 13:39:22 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
#define HISTORY_H

#include "minishell.h"

bool	handle_history(t_mini_data *data);
int		add_to_hist_file(char *input);
int		clear_hist_file(void);
void	print_history(void);
char	*get_history_file_path(void);
void	load_history(void);
void	sanitize_input(char *input);
int 	ft_isascii(int c); 


#endif  // HISTORY_H_
