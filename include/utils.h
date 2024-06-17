/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:38:35 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 09:20:58 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

int     ft_isascii(const int c);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strjoin3(const char *key, const char *mid, const char *value);

#endif  // UTILS_H_
