/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:51:41 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/17 10:40:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include <stdbool.h>

# define HIST_FILE "/tmp/splash/minishell_history"

typedef struct s_data	t_data;	

bool	handle_history(const char *input);
bool	add_to_hist_file(const char *input);
int		clear_hist_file(void);
int		print_history(void);
void	load_history(void);
bool	sanitize_input(const char *input);

#endif  // HISTORY_H_
