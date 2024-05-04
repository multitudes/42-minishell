/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_path.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 17:48:29 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/22 12:15:12 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLE_PATH_H
# define HANDLE_PATH_H

# include "minishell.h"
// forward declaration
typedef struct s_mini_data t_mini_data;

t_list	*get_path_linked_list(char *path);
void	print_path_list(t_list *path_list);
bool	add_path(t_mini_data *data);

#endif