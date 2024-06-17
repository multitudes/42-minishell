/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:38:35 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 15:58:40 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

int     ft_isascii(const int c);
char	*ft_strndup(const char *s, size_t n);
int	    count_char_in_str(char *str, char c);

#endif  // UTILS_H_
